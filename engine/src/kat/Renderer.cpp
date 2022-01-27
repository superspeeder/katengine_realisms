#include "Renderer.h"

namespace kat {

    Renderer::Renderer(std::shared_ptr<App> app) : m_App(app) {
        for (size_t i = 0 ; i < kMaxFramesInFlight; i++) {
            m_InFlightFences.push_back(app->getDevice().createFence(vk::FenceCreateInfo{vk::FenceCreateFlagBits::eSignaled}));
            m_RenderFinishedSemaphores.push_back(app->getDevice().createSemaphore(vk::SemaphoreCreateInfo{}));
            m_ImageAvailableSemaphores.push_back(app->getDevice().createSemaphore(vk::SemaphoreCreateInfo{}));
        }


    }

    Renderer::~Renderer() {

    }

    void Renderer::render() {
        m_App->getDevice().waitForFences(m_InFlightFences[m_CurrentFrame], true, UINT64_MAX);

        uint32_t imgIdx;
        m_App->getDevice().acquireNextImageKHR(m_App->getSwapchain(), UINT64_MAX, m_ImageAvailableSemaphores[m_CurrentFrame])
    }

}
