#include "kat/Engine.h"

#include <iostream>
#include <spdlog/spdlog.h>
#include <algorithm>

void error_callback(int error, const char* description) {
    spdlog::error("Error: {}", description);
}

namespace kat {
    Engine::Engine() {
        spdlog::info("Initializing KatEngine {}", to_string(VERSION));
    }

    Engine::~Engine() {
        spdlog::info("Cleaned up KatEngine {}", to_string(VERSION));
    }

    void Engine::runApp(const std::shared_ptr<App> &app_) {
        m_RunningApp = app_;

        // start to run App;

        init();

        m_RunningApp->setEngine(this);

        m_RunningApp->setupApp();

        while (m_RunningApp->isRunning()) {
            glfwPollEvents();
            m_RunningApp->updateApp();
        }

        m_RunningApp->cleanupApp();

    }

    const std::shared_ptr<App> &Engine::getRunningApp() const noexcept {
        return m_RunningApp;
    }

    void Engine::init() {
        glfwSetErrorCallback(error_callback);

        if (!glfwInit()) {
            spdlog::error("Failed to initialize GLFW");
        }

        vk::InstanceCreateInfo icreateInfo{};
        vk::ApplicationInfo appInfo{};
        appInfo.apiVersion = VK_API_VERSION_1_2;
        appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
        appInfo.pApplicationName = "App";
        appInfo.pEngineName = "KatEngine";

        std::vector<const char*> extensions;
        uint32_t req_count;
        const char** req_exts = glfwGetRequiredInstanceExtensions(&req_count);
        for (uint32_t i = 0 ; i < req_count ; i++) {
            extensions.push_back(req_exts[i]);
        }

        icreateInfo.setPEnabledExtensionNames(extensions).setPApplicationInfo(&appInfo);

        m_Instance = vk::createInstance(icreateInfo);
        spdlog::info("Created Instance!");

        m_Gpu = m_Instance.enumeratePhysicalDevices()[0];
        m_GpuProperties = m_Gpu.getProperties();
        m_GpuFeatures = m_Gpu.getFeatures();
        spdlog::info("Found GPU: {}", m_GpuProperties.deviceName);
        spdlog::info("GPU Type: {}", vk::to_string(m_GpuProperties.deviceType));
        spdlog::info("GPU Vendor ID: {}", m_GpuProperties.vendorID);
        spdlog::info("GPU Device ID: {}", m_GpuProperties.deviceID);
        spdlog::info("GPU Supported API Version : {}", m_GpuProperties.apiVersion);
        spdlog::info("GPU Driver Version: {}", m_GpuProperties.driverVersion);
        spdlog::info("Max 1D image size: {}", m_GpuProperties.limits.maxImageDimension1D);
        spdlog::info("Max 2D image size: {}", m_GpuProperties.limits.maxImageDimension2D);
        spdlog::info("Max 3D image size: {}", m_GpuProperties.limits.maxImageDimension3D);
        spdlog::info("Max Cube image size: {}", m_GpuProperties.limits.maxImageDimensionCube);
        spdlog::info("Max image array layers: {}", m_GpuProperties.limits.maxImageArrayLayers);
        spdlog::info("Max texel buffer elements: {}", m_GpuProperties.limits.maxTexelBufferElements);
        spdlog::info("Max uniform buffer range: {}", m_GpuProperties.limits.maxUniformBufferRange);
        spdlog::info("Max storage buffer range: {}", m_GpuProperties.limits.maxStorageBufferRange);
        spdlog::info("Max push constants size: {}", m_GpuProperties.limits.maxPushConstantsSize);
        spdlog::info("Max memory allocation count: {}", m_GpuProperties.limits.maxMemoryAllocationCount);
        spdlog::info("Max sampler allocation count: {}", m_GpuProperties.limits.maxSamplerAllocationCount);
        spdlog::info("Buffer image granularity: {}", m_GpuProperties.limits.bufferImageGranularity);
        spdlog::info("Sparse address space size: {}", m_GpuProperties.limits.sparseAddressSpaceSize);
        spdlog::info("Max bound descriptor sets: {}", m_GpuProperties.limits.maxBoundDescriptorSets);
        spdlog::info("Max per stage descriptor samplers: {}", m_GpuProperties.limits.maxPerStageDescriptorSamplers);
        spdlog::info("Max per stage descriptor uniform buffers: {}", m_GpuProperties.limits.maxPerStageDescriptorUniformBuffers);
        spdlog::info("Max per stage descriptor storage buffers: {}", m_GpuProperties.limits.maxPerStageDescriptorStorageBuffers);
        spdlog::info("Max per stage descriptor sampled images: {}", m_GpuProperties.limits.maxPerStageDescriptorSampledImages);
        spdlog::info("Max per stage descriptor storage images: {}", m_GpuProperties.limits.maxPerStageDescriptorStorageImages);
        spdlog::info("Max per stage descriptor input attachments: {}", m_GpuProperties.limits.maxPerStageDescriptorInputAttachments);
        spdlog::info("Max per stage resources: {}", m_GpuProperties.limits.maxPerStageResources);
        spdlog::info("Max descriptor set samplers: {}", m_GpuProperties.limits.maxDescriptorSetSamplers);
        spdlog::info("Max descriptor set uniform buffers: {}", m_GpuProperties.limits.maxDescriptorSetUniformBuffers);
        spdlog::info("Max descriptor set uniform buffers dynamic: {}", m_GpuProperties.limits.maxDescriptorSetUniformBuffersDynamic);
        spdlog::info("Max descriptor set storage buffers: {}", m_GpuProperties.limits.maxDescriptorSetStorageBuffers);
        spdlog::info("Max descriptor set storage buffers dynamic: {}", m_GpuProperties.limits.maxDescriptorSetStorageBuffersDynamic);
        spdlog::info("Max descriptor set sampled images: {}", m_GpuProperties.limits.maxDescriptorSetSampledImages);
        spdlog::info("Max descriptor set storage images: {}", m_GpuProperties.limits.maxDescriptorSetStorageImages);
        spdlog::info("Max descriptor set input attachments: {}", m_GpuProperties.limits.maxDescriptorSetInputAttachments);
        spdlog::info("Max vertex input attributes: {}", m_GpuProperties.limits.maxVertexInputAttributes);
        spdlog::info("Max vertex input bindings: {}", m_GpuProperties.limits.maxVertexInputBindings);
        spdlog::info("Max vertex input attribute offset: {}", m_GpuProperties.limits.maxVertexInputAttributeOffset);
        spdlog::info("Max vertex input binding stride: {}", m_GpuProperties.limits.maxVertexInputBindingStride);
        spdlog::info("Max vertex output components: {}", m_GpuProperties.limits.maxVertexOutputComponents);
        spdlog::info("Max tessellation generation level: {}", m_GpuProperties.limits.maxTessellationGenerationLevel);
        spdlog::info("Max tessellation patch size: {}", m_GpuProperties.limits.maxTessellationPatchSize);
        spdlog::info("Max tessellation control per vertex input components: {}", m_GpuProperties.limits.maxTessellationControlPerVertexInputComponents);
        spdlog::info("Max tessellation control per vertex output components: {}", m_GpuProperties.limits.maxTessellationControlPerVertexOutputComponents);
        spdlog::info("Max tessellation control per patch output components: {}", m_GpuProperties.limits.maxTessellationControlPerPatchOutputComponents);
        spdlog::info("Max tessellation control total output components: {}", m_GpuProperties.limits.maxTessellationControlTotalOutputComponents);
        spdlog::info("Max tessellation evaluation input components: {}", m_GpuProperties.limits.maxTessellationEvaluationInputComponents);
        spdlog::info("Max tessellation evaluation output components: {}", m_GpuProperties.limits.maxTessellationEvaluationOutputComponents);
        spdlog::info("Max geometry shader invocations: {}", m_GpuProperties.limits.maxGeometryShaderInvocations);
        spdlog::info("Max geometry input components: {}", m_GpuProperties.limits.maxGeometryInputComponents);
        spdlog::info("Max geometry output components: {}", m_GpuProperties.limits.maxGeometryOutputComponents);
        spdlog::info("Max geometry output vertices: {}", m_GpuProperties.limits.maxGeometryOutputVertices);
        spdlog::info("Max geometry total output components: {}", m_GpuProperties.limits.maxGeometryTotalOutputComponents);
        spdlog::info("Max fragment input components: {}", m_GpuProperties.limits.maxFragmentInputComponents);
        spdlog::info("Max fragment output attachments: {}", m_GpuProperties.limits.maxFragmentOutputAttachments);
        spdlog::info("Max fragment dual source attachments: {}", m_GpuProperties.limits.maxFragmentDualSrcAttachments);
        spdlog::info("Max fragment combined output resources: {}", m_GpuProperties.limits.maxFragmentCombinedOutputResources);
        spdlog::info("Max compute shared memory size: {}", m_GpuProperties.limits.maxComputeSharedMemorySize);
        spdlog::info("Max compute work group count: {} x {} x {}", m_GpuProperties.limits.maxComputeWorkGroupCount[0],m_GpuProperties.limits.maxComputeWorkGroupCount[1],m_GpuProperties.limits.maxComputeWorkGroupCount[2]);
        spdlog::info("Max compute work group invocations: {}", m_GpuProperties.limits.maxComputeWorkGroupInvocations);
        spdlog::info("Max compute work group size: {} x {} x {}", m_GpuProperties.limits.maxComputeWorkGroupSize[0],m_GpuProperties.limits.maxComputeWorkGroupSize[1],m_GpuProperties.limits.maxComputeWorkGroupSize[2]);
        spdlog::info("Sub-pixel precision bits : {}", m_GpuProperties.limits.subPixelPrecisionBits);
        spdlog::info("Sub-texel precision bits : {}", m_GpuProperties.limits.subTexelPrecisionBits);
        spdlog::info("Mipmap precision bits : {}", m_GpuProperties.limits.mipmapPrecisionBits);
        spdlog::info("Max draw indexed index value: {}", m_GpuProperties.limits.maxDrawIndexedIndexValue);
        spdlog::info("Max draw indirect count: {}", m_GpuProperties.limits.maxDrawIndirectCount);
        spdlog::info("Max sampler LOD bias: {}", m_GpuProperties.limits.maxSamplerLodBias);
        spdlog::info("Max sampler anisotropy: {}", m_GpuProperties.limits.maxSamplerAnisotropy);
        spdlog::info("Max viewports: {}", m_GpuProperties.limits.maxViewports);
        spdlog::info("Max viewport size: {} x {}", m_GpuProperties.limits.maxViewportDimensions[0], m_GpuProperties.limits.maxViewportDimensions[1]);
        spdlog::info("Viewport bounds: {} -> {}", m_GpuProperties.limits.viewportBoundsRange[0], m_GpuProperties.limits.viewportBoundsRange[1]);
        spdlog::info("Viewport sub-pixel bits : {}", m_GpuProperties.limits.viewportSubPixelBits);
        spdlog::info("Min memory map alignment : {}", m_GpuProperties.limits.minMemoryMapAlignment);
        spdlog::info("Min texel buffer offset alignment : {}", m_GpuProperties.limits.minTexelBufferOffsetAlignment);
        spdlog::info("Min uniform buffer offset alignment : {}", m_GpuProperties.limits.minUniformBufferOffsetAlignment);
        spdlog::info("Min storage buffer offset alignment : {}", m_GpuProperties.limits.minStorageBufferOffsetAlignment);
        spdlog::info("Min texel offset: {}", m_GpuProperties.limits.minTexelOffset);
        spdlog::info("Max texel offset: {}", m_GpuProperties.limits.maxTexelOffset);
        spdlog::info("Min texel gather offset: {}", m_GpuProperties.limits.minTexelGatherOffset);
        spdlog::info("Max texel gather offset: {}", m_GpuProperties.limits.maxTexelGatherOffset);
        spdlog::info("Min interpolation offset: {}", m_GpuProperties.limits.minInterpolationOffset);
        spdlog::info("Max interpolation offset: {}", m_GpuProperties.limits.maxInterpolationOffset);
        spdlog::info("Sub-pixel interpolation offset bits: {}", m_GpuProperties.limits.subPixelInterpolationOffsetBits);
        spdlog::info("Max framebuffer width: {}", m_GpuProperties.limits.maxFramebufferWidth);
        spdlog::info("Max framebuffer height: {}", m_GpuProperties.limits.maxFramebufferHeight);
        spdlog::info("Max framebuffer layers: {}", m_GpuProperties.limits.maxFramebufferLayers);
        spdlog::info("Framebuffer color sample counts: {}", vk::to_string(m_GpuProperties.limits.framebufferColorSampleCounts));
        spdlog::info("Framebuffer depth sample counts: {}", vk::to_string(m_GpuProperties.limits.framebufferDepthSampleCounts));
        spdlog::info("Framebuffer stencil sample counts: {}", vk::to_string(m_GpuProperties.limits.framebufferStencilSampleCounts));
        spdlog::info("Framebuffer no attachments sample counts: {}", vk::to_string(m_GpuProperties.limits.framebufferNoAttachmentsSampleCounts));
        spdlog::info("Max color attachments: {}", m_GpuProperties.limits.maxColorAttachments);
        spdlog::info("Sampled image color sample counts: {}", vk::to_string(m_GpuProperties.limits.sampledImageColorSampleCounts));
        spdlog::info("Sampled image integer sample counts: {}", vk::to_string(m_GpuProperties.limits.sampledImageIntegerSampleCounts));
        spdlog::info("Sampled image depth sample counts: {}", vk::to_string(m_GpuProperties.limits.sampledImageDepthSampleCounts));
        spdlog::info("Sampled image stencil sample counts: {}", vk::to_string(m_GpuProperties.limits.sampledImageStencilSampleCounts));
        spdlog::info("Storage image sample counts: {}", vk::to_string(m_GpuProperties.limits.storageImageSampleCounts));
        spdlog::info("Max sample mask words: {}", m_GpuProperties.limits.maxSampleMaskWords);
        spdlog::info("Timestamp compute and graphics: {}", m_GpuProperties.limits.timestampComputeAndGraphics ? "True" : "False");
        spdlog::info("Timestamp period: {}", m_GpuProperties.limits.timestampPeriod);
        spdlog::info("Max clip distances: {}", m_GpuProperties.limits.maxClipDistances);
        spdlog::info("Max cull distances: {}", m_GpuProperties.limits.maxCullDistances);
        spdlog::info("Max combined clip and cull distances: {}", m_GpuProperties.limits.maxCombinedClipAndCullDistances);
        spdlog::info("Max point size: {} -> {}", m_GpuProperties.limits.pointSizeRange[0], m_GpuProperties.limits.pointSizeRange[1]);
        spdlog::info("Max line width: {} -> {}", m_GpuProperties.limits.lineWidthRange[0], m_GpuProperties.limits.lineWidthRange[1]);
        spdlog::info("Point size granularity: {}", m_GpuProperties.limits.pointSizeGranularity);
        spdlog::info("Line width granularity: {}", m_GpuProperties.limits.lineWidthGranularity);
        spdlog::info("Strict lines: {}", m_GpuProperties.limits.strictLines ? "True" : "False");
        spdlog::info("Standard sample locations: {}", m_GpuProperties.limits.standardSampleLocations ? "True" : "False");
        spdlog::info("Discrete queue priorities: {}", m_GpuProperties.limits.discreteQueuePriorities);
        spdlog::info("Optimal buffer copy offset alignment: {}", m_GpuProperties.limits.optimalBufferCopyOffsetAlignment);
        spdlog::info("Optimal buffer copy row pitch alignment: {}", m_GpuProperties.limits.optimalBufferCopyRowPitchAlignment);
        spdlog::info("Non-coherent atom size: {}", m_GpuProperties.limits.nonCoherentAtomSize);
        spdlog::info("GPU Features:");
        spdlog::info("- Robust buffer access: {}", m_GpuFeatures.robustBufferAccess ? "True" : "False");
        spdlog::info("- Full draw index uint32: {}", m_GpuFeatures.fullDrawIndexUint32 ? "True" : "False");
        spdlog::info("- Image cube array: {}", m_GpuFeatures.imageCubeArray ? "True" : "False");
        spdlog::info("- Independent blend: {}", m_GpuFeatures.independentBlend ? "True" : "False");
        spdlog::info("- Geometry shader: {}", m_GpuFeatures.geometryShader ? "True" : "False");
        spdlog::info("- Tessellation shader: {}", m_GpuFeatures.tessellationShader ? "True" : "False");
        spdlog::info("- Sample rate shading: {}", m_GpuFeatures.sampleRateShading ? "True" : "False");
        spdlog::info("- Dual source blend: {}", m_GpuFeatures.dualSrcBlend ? "True" : "False");
        spdlog::info("- Logic Ops: {}", m_GpuFeatures.logicOp ? "True" : "False");
        spdlog::info("- Multi draw indirect: {}", m_GpuFeatures.multiDrawIndirect ? "True" : "False");
        spdlog::info("- Draw indirect first instance: {}", m_GpuFeatures.drawIndirectFirstInstance ? "True" : "False");
        spdlog::info("- Depth clamp: {}", m_GpuFeatures.depthClamp ? "True" : "False");
        spdlog::info("- Depth bias clamp: {}", m_GpuFeatures.depthBiasClamp ? "True" : "False");
        spdlog::info("- Fill mode non solid: {}", m_GpuFeatures.fillModeNonSolid ? "True" : "False");
        spdlog::info("- Depth bounds: {}", m_GpuFeatures.depthBounds ? "True" : "False");
        spdlog::info("- Wide lines: {}", m_GpuFeatures.wideLines ? "True" : "False");
        spdlog::info("- Large points: {}", m_GpuFeatures.largePoints ? "True" : "False");
        spdlog::info("- Alpha to one: {}", m_GpuFeatures.alphaToOne ? "True" : "False");
        spdlog::info("- Multi viewport: {}", m_GpuFeatures.multiViewport ? "True" : "False");
        spdlog::info("- Sampler anisotropy: {}", m_GpuFeatures.samplerAnisotropy ? "True" : "False");
        spdlog::info("- Texture compression ETC2: {}", m_GpuFeatures.textureCompressionETC2 ? "True" : "False");
        spdlog::info("- Texture compression ASTC_LDR: {}", m_GpuFeatures.textureCompressionASTC_LDR ? "True" : "False");
        spdlog::info("- Texture Compression BC: {}", m_GpuFeatures.textureCompressionBC ? "True" : "False");
        spdlog::info("- Occlusion query precise: {}", m_GpuFeatures.occlusionQueryPrecise ? "True" : "False");
        spdlog::info("- Pipeline statistics query: {}", m_GpuFeatures.pipelineStatisticsQuery ? "True" : "False");
        spdlog::info("- Vertex pipeline stores and atomics: {}", m_GpuFeatures.vertexPipelineStoresAndAtomics ? "True" : "False");
        spdlog::info("- Fragment stores and atomics: {}", m_GpuFeatures.fragmentStoresAndAtomics ? "True" : "False");
        spdlog::info("- Shader tessellation and geometry point size: {}", m_GpuFeatures.shaderTessellationAndGeometryPointSize ? "True" : "False");
        spdlog::info("- Shader image gather extended: {}", m_GpuFeatures.shaderImageGatherExtended ? "True" : "False");
        spdlog::info("- Shader storage image extended formats: {}", m_GpuFeatures.shaderStorageImageExtendedFormats ? "True" : "False");
        spdlog::info("- Shader storage image multisample: {}", m_GpuFeatures.shaderStorageImageMultisample ? "True" : "False");
        spdlog::info("- Shader storage image read without format: {}", m_GpuFeatures.shaderStorageImageReadWithoutFormat ? "True" : "False");
        spdlog::info("- Shader storage image write without format: {}", m_GpuFeatures.shaderStorageImageWriteWithoutFormat ? "True" : "False");
        spdlog::info("- Shader uniform buffer array dynamic indexing: {}", m_GpuFeatures.shaderUniformBufferArrayDynamicIndexing ? "True" : "False");
        spdlog::info("- Shader sampled image array dynamic indexing: {}", m_GpuFeatures.shaderSampledImageArrayDynamicIndexing ? "True" : "False");
        spdlog::info("- Shader storage buffer array dynamic indexing: {}", m_GpuFeatures.shaderStorageBufferArrayDynamicIndexing ? "True" : "False");
        spdlog::info("- Shader storage image array dynamic indexing: {}", m_GpuFeatures.shaderStorageImageArrayDynamicIndexing ? "True" : "False");
        spdlog::info("- Shader clip distance: {}", m_GpuFeatures.shaderClipDistance ? "True" : "False");
        spdlog::info("- Shader cull distance: {}", m_GpuFeatures.shaderCullDistance ? "True" : "False");
        spdlog::info("- Shader doubles: {}", m_GpuFeatures.shaderFloat64 ? "True" : "False");
        spdlog::info("- Shader longs: {}", m_GpuFeatures.shaderInt64 ? "True" : "False");
        spdlog::info("- Shader shorts: {}", m_GpuFeatures.shaderInt16 ? "True" : "False");
        spdlog::info("- Shader resource residency: {}", m_GpuFeatures.shaderResourceResidency ? "True" : "False");
        spdlog::info("- Shader resource min LODs: {}", m_GpuFeatures.shaderResourceMinLod ? "True" : "False");
        spdlog::info("- Sparse binding: {}", m_GpuFeatures.sparseBinding ? "True" : "False");
        spdlog::info("- Sparse residency buffer: {}", m_GpuFeatures.sparseResidencyBuffer ? "True" : "False");
        spdlog::info("- Sparse residency image 2D: {}", m_GpuFeatures.sparseResidencyImage2D ? "True" : "False");
        spdlog::info("- Sparse residency image 3D: {}", m_GpuFeatures.sparseResidencyImage3D ? "True" : "False");
        spdlog::info("- Sparse residency 2x samples: {}", m_GpuFeatures.sparseResidency2Samples ? "True" : "False");
        spdlog::info("- Sparse residency 4x samples: {}", m_GpuFeatures.sparseResidency4Samples ? "True" : "False");
        spdlog::info("- Sparse residency 8x samples: {}", m_GpuFeatures.sparseResidency8Samples ? "True" : "False");
        spdlog::info("- Sparse residency 16x samples: {}", m_GpuFeatures.sparseResidency16Samples ? "True" : "False");
        spdlog::info("- Sparse residency aliased: {}", m_GpuFeatures.sparseResidencyAliased ? "True" : "False");
        spdlog::info("- Variable multisample rate: {}", m_GpuFeatures.variableMultisampleRate ? "True" : "False");
        spdlog::info("- Inherited queries: {}", m_GpuFeatures.inheritedQueries ? "True" : "False");

        m_SupportedDeviceExtensions = m_Gpu.enumerateDeviceExtensionProperties();
        m_SupportedDeviceLayers = m_Gpu.enumerateDeviceLayerProperties();

        spdlog::info("Supported Device Extensions:");
        for (const auto& ext : m_SupportedDeviceExtensions) {
            spdlog::info("- {}", ext.extensionName);
            m_SupportedDeviceExtensionNames.insert(ext.extensionName);
        }

        spdlog::info("Supported Device Layers:");
        for (const auto& lyr : m_SupportedDeviceLayers) {
            spdlog::info("- {}", lyr.layerName);
            m_SupportedDeviceLayerNames.insert(lyr.layerName);
        }
    }

    void Engine::cleanup() {
        m_Instance.destroy();

        glfwTerminate();
    }

    bool Engine::supportsExtension(const std::string &name) const {
        return m_SupportedDeviceExtensionNames.contains(name);
    }

    bool Engine::supportsLayer(const std::string &name) const {
        return m_SupportedDeviceLayerNames.contains(name);
    }

    vk::Instance Engine::getInstance() {
        return m_Instance;
    }

    const vk::PhysicalDevice &Engine::getGpu() const {
        return m_Gpu;
    }

    const vk::PhysicalDeviceFeatures &Engine::getGpuFeatures() const {
        return m_GpuFeatures;
    }

    std::string to_string(const version &ver) {
        return std::to_string(ver.major) + "." + std::to_string(ver.minor) + "." + std::to_string(ver.patch);
    }

    void App::setEngine(Engine *engine) {
        m_Engine = engine;
    }

    App::App() {
    }

    App::~App() {

    }



    void App::setupApp() {
        m_Running = true;

        glfwDefaultWindowHints();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        switch (m_Configuration.window_mode.index()) {
            case 0: {
                WindowedWindowMode mode = std::get<WindowedWindowMode>(m_Configuration.window_mode);

                glfwWindowHint(GLFW_FLOATING, mode.floating);
                glfwWindowHint(GLFW_RESIZABLE, mode.resizable);

                m_Window = glfwCreateWindow(mode.size.x, mode.size.y, m_Configuration.app_name.c_str(), nullptr, nullptr);
            }
            break;
            case 1: {
                FullscreenWindowMode mode = std::get<FullscreenWindowMode>(m_Configuration.window_mode);
                int32_t monitor_count;
                GLFWmonitor** monitors = glfwGetMonitors(&monitor_count);
                size_t mid = std::clamp(mode.monitor_id, 0ULL, static_cast<size_t>(monitor_count - 1));
                GLFWmonitor* monitor = monitors[mid];
                const GLFWvidmode* vmode = glfwGetVideoMode(monitor);

                glfwWindowHint(GLFW_RED_BITS, vmode->redBits);
                glfwWindowHint(GLFW_GREEN_BITS, vmode->greenBits);
                glfwWindowHint(GLFW_BLUE_BITS, vmode->blueBits);
                glfwWindowHint(GLFW_REFRESH_RATE, vmode->refreshRate);

                m_Window = glfwCreateWindow(vmode->width, vmode->height, m_Configuration.app_name.c_str(), monitor, nullptr);
            }
            break;
        }

        VkSurfaceKHR srf_;
        glfwCreateWindowSurface(m_Engine->getInstance(), m_Window, nullptr, &srf_);
        m_Surface = srf_;

        std::vector<const char*> dev_exts = {
                VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };

        for (const char*& ext_ : dev_exts) {
            if (!m_Engine->supportsExtension(ext_)) {
                spdlog::error("ERROR: GPU DOES NOT SUPPORT EXTENSION '{}'", ext_);
                throw std::runtime_error("Unsupported device extension requested");
            }
        }

        vk::DeviceCreateInfo dci{};
        dci.setPEnabledExtensionNames(dev_exts);

        auto qfps = m_Engine->getGpu().getQueueFamilyProperties();

        size_t i = 0;
        for (const auto& qf : qfps) {
            spdlog::info("Queue Family #{}:", i);

            spdlog::info("- Compute: {}", qf.queueFlags & vk::QueueFlagBits::eCompute ? "True" : "False");
            spdlog::info("- Graphics: {}", qf.queueFlags & vk::QueueFlagBits::eGraphics ? "True" : "False");
            spdlog::info("- Protected: {}", qf.queueFlags & vk::QueueFlagBits::eProtected ? "True" : "False");
            spdlog::info("- Sparse Binding: {}", qf.queueFlags & vk::QueueFlagBits::eSparseBinding ? "True" : "False");
            spdlog::info("- Transfer: {}", qf.queueFlags & vk::QueueFlagBits::eTransfer ? "True" : "False");

            spdlog::info("- Queue Count: {}", qf.queueCount);
            spdlog::info("- Min Image Transfer Granularity: {} x {} x {}", qf.minImageTransferGranularity.width, qf.minImageTransferGranularity.height, qf.minImageTransferGranularity.depth);
            spdlog::info("- Timestamp Valid Bits: {}", qf.timestampValidBits);

            bool supportsPresentation = m_Engine->getGpu().getSurfaceSupportKHR(i, m_Surface);
            spdlog::info("- Presentation: {}", supportsPresentation ? "True" : "False");

            if (!(m_GraphicsFamily.has_value() && m_PresentFamily.has_value())) {
                if (qf.queueFlags & vk::QueueFlagBits::eGraphics) {
                    m_GraphicsFamily = i;
                }

                if (supportsPresentation) {
                    m_PresentFamily = i;
                }
            }

            i++;
        }

        if (!(m_GraphicsFamily.has_value() && m_PresentFamily.has_value())) {
            spdlog::error("Failed to find required queue families");
            throw std::runtime_error("Failed to find required queue families");
        }

        m_SameQueueFamily = m_GraphicsFamily.value() == m_PresentFamily.value();

        std::vector<vk::DeviceQueueCreateInfo> dqcis;

        float qp = 1.0f;

        if (m_SameQueueFamily) {
            dqcis.emplace_back(
                vk::DeviceQueueCreateFlags(), m_GraphicsFamily.value(), 1, &qp
            );
        } else {
            dqcis.emplace_back(
                    vk::DeviceQueueCreateFlags(), m_GraphicsFamily.value(), 1, &qp
            );
            dqcis.emplace_back(
                    vk::DeviceQueueCreateFlags(), m_PresentFamily.value(), 1, &qp
            );
        }

        dci.setQueueCreateInfos(dqcis);
        dci.setPEnabledFeatures(&m_Engine->getGpuFeatures());

        m_Device = m_Engine->getGpu().createDevice(dci);
        spdlog::info("Created logical device");

        m_GraphicsQueue = m_Device.getQueue(m_GraphicsFamily.value(), 0);
        m_PresentQueue = m_Device.getQueue(m_PresentFamily.value(), 0);

        vk::SwapchainCreateInfoKHR sci{};

        vk::SurfaceCapabilitiesKHR scaps = m_Engine->getGpu().getSurfaceCapabilitiesKHR(m_Surface);

        sci.clipped = true;
        sci.compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque;
        sci.imageArrayLayers = 1;
        sci.imageSharingMode = m_SameQueueFamily ? vk::SharingMode::eExclusive : vk::SharingMode::eConcurrent;
        std::vector<uint32_t> qfs_sc{m_GraphicsFamily.value(), m_PresentFamily.value()};
        if (!m_SameQueueFamily) {
            sci.setQueueFamilyIndices(qfs_sc);
        }
        sci.imageUsage = vk::ImageUsageFlagBits::eColorAttachment;
        sci.preTransform = scaps.currentTransform;
        sci.minImageCount = scaps.maxImageCount == 0 ? scaps.minImageCount + 1 : std::min(scaps.minImageCount + 1, scaps.maxImageCount);
        sci.surface = m_Surface;

        auto fmts = m_Engine->getGpu().getSurfaceFormatsKHR(m_Surface);
        auto pms = m_Engine->getGpu().getSurfacePresentModesKHR(m_Surface);

        auto fm = fmts[0];
        for (const auto& f : fmts) {
            if (f.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear && f.format == vk::Format::eB8G8R8A8Srgb) {
                fm = f;
            }
        }

        m_SwapchainFormat = fm.format;

        sci.imageFormat = fm.format;
        sci.imageColorSpace = fm.colorSpace;

        sci.presentMode = vk::PresentModeKHR::eFifo;

        for (const auto& pm : pms) {
            if (pm == vk::PresentModeKHR::eMailbox) {
                sci.presentMode = pm;
            }
        }

        m_PresentMode = sci.presentMode;

        int fw, fh;
        glfwGetFramebufferSize(m_Window, &fw, &fh);

        sci.imageExtent = scaps.currentExtent.width == UINT32_MAX ? vk::Extent2D{
                std::clamp(static_cast<uint32_t>(fw), scaps.minImageExtent.width, scaps.maxImageExtent.width),
                std::clamp(static_cast<uint32_t>(fh), scaps.minImageExtent.height, scaps.maxImageExtent.height)
        } : scaps.currentExtent;

        m_SwapchainExtent = sci.imageExtent;

        m_Swapchain = m_Device.createSwapchainKHR(sci);
        spdlog::info("Created Swapchain");

        m_SwapchainImages = m_Device.getSwapchainImagesKHR(m_Swapchain);
        spdlog::info("Obtained {} images from swapchain", m_SwapchainImages.size());

        for (const auto& img : m_SwapchainImages) {
            m_SwapchainImageViews.push_back(m_Device.createImageView(vk::ImageViewCreateInfo{
                vk::ImageViewCreateFlags(), img, vk::ImageViewType::e2D, m_SwapchainFormat, vk::ComponentMapping{
                            vk::ComponentSwizzle::eR,
                            vk::ComponentSwizzle::eG,
                            vk::ComponentSwizzle::eB,
                            vk::ComponentSwizzle::eA
                }, vk::ImageSubresourceRange{
                    vk::ImageAspectFlags(), 0, 1, 0, 1
                }
            }));
        }
        spdlog::info("Created {} image views for swapchain images", m_SwapchainImageViews.size());

        setup();
    }

    bool App::isRunning() const noexcept {
        return m_Running;
    }

    void App::stop() {
        m_Running = false;
    }

    void App::updateApp() {
        m_Clock.nextFrame();

        update(m_Clock.getFrameTime().count());

        if (glfwWindowShouldClose(m_Window)) {
            stop();
        }
    }

    void App::cleanupApp() {
        cleanup();

        for (const auto& siv : m_SwapchainImageViews) {
            m_Device.destroyImageView(siv);
        }
        m_Device.destroySwapchainKHR(m_Swapchain);
        m_Device.destroy();

        glfwDestroyWindow(m_Window);
    }

    vk::Device App::getDevice() {
        return m_Device;
    }

    vk::SwapchainKHR App::getSwapchain() {
        return m_Swapchain;
    }

    vk::Queue App::getGraphicsQueue() {
        return m_GraphicsQueue;
    }

    vk::Queue App::getPresentQueue() {
        return m_PresentQueue;
    }

    uint32_t App::getGraphicsFamily() {
        return m_GraphicsFamily.value();
    }

    uint32_t App::getPresentFamily() {
        return m_PresentFamily.value();
    }

    std::vector<vk::Image> App::getSwapchainImages() {
        return m_SwapchainImages;
    }

    std::vector<vk::ImageView> App::getSwapchainImageViews() {
        return m_SwapchainImageViews;
    }

    vk::Format App::getSwapchainFormat() {
        return m_SwapchainFormat;
    }

    vk::Extent2D App::getSwapchainExtent() {
        return m_SwapchainExtent;
    }

    vk::PresentModeKHR App::getPresentMode() {
        return m_PresentMode;
    }

    AppClock::time_point AppClock::getStartTime() {
        return startTime;
    }

    AppClock::time_point AppClock::getLastFrame() {
        return lastFrame;
    }

    AppClock::time_point AppClock::now() {
        return clock::now();
    }

    AppClock::duration AppClock::getUptime() {
        return now() - startTime;
    }

    AppClock::duration AppClock::getFrameTime() {
        return lastFrameTime;
    }

    double AppClock::getFramesPerSecond() {
        return lastFpsExact;
    }

    double AppClock::getSmoothedFramesPerSecond() {
        return lastFpsSmooth;
    }

    double AppClock::getAverageFramesPerSecond() {
        return (double)frameCount / getUptime().count();
    }

    size_t AppClock::getFrameCount() {
        return frameCount;
    }

    void AppClock::nextFrame() {
        time_point thisFrame = now();
        lastFrameTime = thisFrame - lastFrame;
        lastFpsExact = 1.0 / getFrameTime().count();
        lastFpsSmooth = (lastFpsSmooth * kSmoothingConstant) + (lastFpsExact * (1.0 - kSmoothingConstant));
        lastFrame = thisFrame;
        frameCount += 1;

    }

    AppClock::AppClock() {
        startTime = now();

    }

    FullscreenWindowMode::FullscreenWindowMode(size_t monitor_id_) : monitor_id{monitor_id_} {
    }
}