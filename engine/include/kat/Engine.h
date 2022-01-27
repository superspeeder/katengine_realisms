#pragma once

#include <cinttypes>
#include <optional>
#include <memory>
#include <string>
#include <chrono>
#include <concepts>
#include <glm/glm.hpp>
#include <variant>
#include <unordered_set>
#include "vulkan/vulkan.hpp"
#include <GLFW/glfw3.h>

namespace kat {

    struct version {
        uint32_t major,minor,patch;
    };

    class AppClock {
    public:
        using clock = std::chrono::steady_clock;
        using duration = std::chrono::duration<double>;
        using time_point = std::chrono::time_point<clock, duration>;

        AppClock();

        time_point getStartTime();
        time_point getLastFrame();
        time_point now();

        duration getUptime();
        duration getFrameTime();
        double getFramesPerSecond();
        double getSmoothedFramesPerSecond();
        double getAverageFramesPerSecond();
        size_t getFrameCount();

        void nextFrame();

    private:

        static constexpr double kSmoothingConstant = 0.9;

        time_point startTime, lastFrame;
        duration lastFrameTime;
        size_t frameCount = 0;
        double lastFpsExact;
        double lastFpsSmooth;
    };

    struct WindowedWindowMode {
        glm::ivec2 size{800, 800};
        bool resizable = false;
        bool floating = false;
    };

    struct FullscreenWindowMode {
        explicit FullscreenWindowMode(size_t monitor_id);

        size_t monitor_id = 0;
    };

    struct AppConfig {
        std::string app_name = "App";
        version app_version{0, 0, 1};

        std::variant<WindowedWindowMode, FullscreenWindowMode> window_mode = FullscreenWindowMode(0);
    };

    class Engine;

    class App {
    public:

        App();
        virtual ~App();

        void setEngine(Engine* engine);

        [[nodiscard]] bool isRunning() const noexcept;

        void stop();

        virtual void setup() = 0;
        virtual void update(double dt) = 0;
        virtual void cleanup() = 0;

        vk::Device getDevice();
        vk::SwapchainKHR getSwapchain();
        vk::Queue getGraphicsQueue();
        vk::Queue getPresentQueue();
        uint32_t getGraphicsFamily();
        uint32_t getPresentFamily();
        std::vector<vk::Image> getSwapchainImages();
        std::vector<vk::ImageView> getSwapchainImageViews();
        vk::Format getSwapchainFormat();
        vk::Extent2D getSwapchainExtent();
        vk::PresentModeKHR getPresentMode();


    protected:
        AppConfig m_Configuration{};
        void setupApp();
        void updateApp();
        void cleanupApp();

        friend class Engine;
        Engine *m_Engine = nullptr;

        AppClock m_Clock;

    private:
        GLFWwindow* m_Window;
        bool m_Running = false;
        vk::SurfaceKHR m_Surface;
        vk::Device m_Device;
        vk::SwapchainKHR m_Swapchain;
        vk::Queue m_GraphicsQueue;
        vk::Queue m_PresentQueue;
        std::optional<uint32_t> m_GraphicsFamily;
        std::optional<uint32_t> m_PresentFamily;
        bool m_SameQueueFamily;

        std::vector<vk::Image> m_SwapchainImages;
        std::vector<vk::ImageView> m_SwapchainImageViews;

        vk::PresentModeKHR m_PresentMode;
        vk::Format m_SwapchainFormat;
        vk::Extent2D m_SwapchainExtent;
    };

    template<typename T>
    concept LaunchableApp = requires { { T() } -> std::same_as<T>; } && std::derived_from<T,App>;

    class Engine {
    public:

        Engine();
        ~Engine();

        void runApp(const std::shared_ptr<App>& app_);
        [[nodiscard]] const std::shared_ptr<App>& getRunningApp() const noexcept;

        template<LaunchableApp T>
        inline void launchApp() {
            runApp(std::make_shared<T>());
        };

        [[nodiscard]] bool supportsExtension(const std::string& name) const;
        [[nodiscard]] bool supportsLayer(const std::string& name) const;

        vk::Instance getInstance();
        [[nodiscard]] const vk::PhysicalDevice &getGpu() const;

        [[nodiscard]] const vk::PhysicalDeviceFeatures &getGpuFeatures() const;

    private:

        std::shared_ptr<App> m_RunningApp;

        vk::Instance m_Instance;
        vk::PhysicalDevice m_Gpu;
        vk::PhysicalDeviceProperties m_GpuProperties;
        vk::PhysicalDeviceFeatures m_GpuFeatures;

        void init();
        void cleanup();

        std::vector<vk::ExtensionProperties> m_SupportedDeviceExtensions;
        std::vector<vk::LayerProperties> m_SupportedDeviceLayers;

        std::unordered_set<std::string> m_SupportedDeviceExtensionNames;
        std::unordered_set<std::string> m_SupportedDeviceLayerNames;
    };

    constexpr version VERSION{0, 0, 1};

    std::string to_string(const version& ver);
}