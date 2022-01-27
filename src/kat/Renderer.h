#pragma once

#include "kat/Engine.h"

namespace kat {
    constexpr size_t kMaxFramesInFlight = 2;

    class Renderer {
    public:

        Renderer(std::shared_ptr<App> app);

        ~Renderer();

        void render();

    private:

        std::shared_ptr<App> m_App;

        std::vector<vk::Semaphore> m_RenderFinishedSemaphores;
        std::vector<vk::Semaphore> m_ImageAvailableSemaphores;
        std::vector<vk::Fence> m_InFlightFences;
        std::vector<vk::Fence> m_ImagesInFlight;
        size_t m_CurrentFrame = 0;

        std::vector<vk::CommandBuffer> m_RenderCommandBuffers;
        vk::CommandPool m_RenderCommandPool;
        vk::RenderPass m_RenderPass;
        vk::Pipeline m_Pipeline;
        vk::PipelineLayout m_PipelineLayout;
        std::vector<vk::Framebuffer> m_Framebuffers;


    };
}