#include "kat/Renderer.h"

namespace kat {

    Renderer::Renderer(std::shared_ptr<App> app) : m_App(app) {
        for (size_t i = 0 ; i < kMaxFramesInFlight; i++) {
            m_InFlightFences.push_back(app->getDevice().createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled}));
            m_ImageAvailableSemaphores.push_back(app->getDevice().createSemaphore(vk::SemaphoreCreateInfo{}));
            m_RenderFinishedSemaphores.push_back(app->getDevice().createSemaphore(vk::SemaphoreCreateInfo{}));
        }

        m_ImagesInFlight.resize(m_App->getSwapchainImages().size());

        m_RenderCommandPool = m_App->getDevice().createCommandPool(vk::CommandPoolCreateInfo{
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer, m_App->getGraphicsFamily()
            });

        m_RenderCommandBuffers = m_App->getDevice().allocateCommandBuffers(vk::CommandBufferAllocateInfo{
            m_RenderCommandPool, vk::CommandBufferLevel::ePrimary, static_cast<uint32_t>(m_App->getSwapchainImages().size())
            });


        vk::AttachmentDescription colorAttachment{
            vk::AttachmentDescriptionFlags(),
            m_App->getSwapchainFormat(),
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR
        };

        vk::AttachmentReference colorAttachmentRef{0, vk::ImageLayout::eColorAttachmentOptimal};
        vk::SubpassDescription subpassDesc{
            vk::SubpassDescriptionFlags(),
            vk::PipelineBindPoint::eGraphics,
            {}, colorAttachmentRef, {}, nullptr, {}
        };

        vk::SubpassDependency subpassDep{
            VK_SUBPASS_EXTERNAL, 0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            {}, vk::AccessFlagBits::eColorAttachmentWrite, {}
        };

        vk::RenderPassCreateInfo rpci{
            vk::RenderPassCreateFlags(),
            colorAttachment,
            subpassDesc,
            subpassDep
        };

        m_RenderPass = m_App->getDevice().createRenderPass(rpci);

        for (auto iview : m_App->getSwapchainImageViews()) {
            m_Framebuffers.push_back(m_App->getDevice().createFramebuffer(vk::FramebufferCreateInfo{
                vk::FramebufferCreateFlags(),
                m_RenderPass,
                iview,
                m_App->getSwapchainExtent().width, m_App->getSwapchainExtent().height, 1
                }));
        }
    }

    Renderer::~Renderer() {

    }

    void Renderer::cleanup() {
        for (size_t i = 0; i < kMaxFramesInFlight; i++) {
            m_App->getDevice().destroyFence(m_InFlightFences[i]);
            m_App->getDevice().destroySemaphore(m_ImageAvailableSemaphores[i]);
            m_App->getDevice().destroySemaphore(m_RenderFinishedSemaphores[i]);
        }

        for (auto fb : m_Framebuffers) {
            m_App->getDevice().destroyFramebuffer(fb);
        }

        m_App->getDevice().destroyRenderPass(m_RenderPass);
        m_App->getDevice().freeCommandBuffers(m_RenderCommandPool, m_RenderCommandBuffers);
        m_App->getDevice().destroyCommandPool(m_RenderCommandPool);
    }

    void Renderer::render() {
        m_App->getDevice().waitForFences(m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);

        uint32_t imgIdx = m_App->getDevice().acquireNextImageKHR(m_App->getSwapchain(), UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame]);
        if (m_ImagesInFlight[imgIdx]) {
            m_App->getDevice().waitForFences(m_ImagesInFlight[imgIdx], true, UINT64_MAX);
        }
        m_ImagesInFlight[imgIdx] = m_InFlightFences[m_CurrentFrame];

        // do the renderings here

        std::vector<vk::Semaphore> waitSemaphores = { m_ImageAvailableSemaphores[m_CurrentFrame] };
        std::vector<vk::Semaphore> signalSemaphores = { m_RenderFinishedSemaphores[m_CurrentFrame] };
        std::vector<vk::PipelineStageFlags> waitStages = { vk::PipelineStageFlagBits::eColorAttachmentOutput };

        vk::CommandBuffer commandBuffer = m_RenderCommandBuffers[m_CurrentFrame];

        vk::SubmitInfo renderSubmit{};
        renderSubmit.setCommandBuffers(commandBuffer);
        renderSubmit.setWaitSemaphores(waitSemaphores);
        renderSubmit.setWaitDstStageMask(waitStages);
        renderSubmit.setSignalSemaphores(signalSemaphores);

        // record new commands

        commandBuffer.begin(vk::CommandBufferBeginInfo{vk::CommandBufferUsageFlagBits::eOneTimeSubmit});
        commandBuffer.beginRenderPass(vk::RenderPassBeginInfo{
            m_RenderPass, m_Framebuffers[imgIdx], vk::Rect2D{
                vk::Offset2D{0, 0}, m_App->getSwapchainExtent()
            }, m_ClearValue }, vk::SubpassContents::eInline);
        commandBuffer.endRenderPass();
        commandBuffer.end();

        // submit queue
        m_App->getDevice().resetFences(m_InFlightFences[m_CurrentFrame]);
        m_App->getGraphicsQueue().submit(renderSubmit, m_InFlightFences[m_CurrentFrame]);

        // done rendering

        // present image
        auto swapchain = m_App->getSwapchain();

        vk::PresentInfoKHR presentInfo{};
        presentInfo.setWaitSemaphores(signalSemaphores);
        presentInfo.setSwapchains(swapchain);
        presentInfo.setImageIndices(imgIdx);

        m_App->getPresentQueue().presentKHR(presentInfo);

        m_CurrentFrame = (m_CurrentFrame + 1) % kMaxFramesInFlight;
    }

}
