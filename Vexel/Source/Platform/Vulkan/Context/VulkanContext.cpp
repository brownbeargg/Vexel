#include "VulkanContext.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"

namespace Vex
{
    namespace
    {
        static constexpr bool s_EnableInfoLogs = true;

        static constexpr bool s_EnableValidationLayers = true;
        std::vector<const char*> s_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

        std::vector<const char*> GetRequiredInstanceExtensions(vk::raii::Context& context);
        std::vector<const char*> GetRequiredInstanceLayers(vk::raii::Context& context);

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

        bool s_InitializedContext = false;
        bool s_InitializedLogicalContext = false;
    } // namespace

    void VulkanContext::Init()
    {
        m_SwapChain = Ref<VulkanSwapChain>::Create(
            s_Context.Instance, s_Context.PhysicalDevice, s_LogicalDevice, m_pWindow);

        if (!s_InitializedLogicalContext)
        {
            CreateLogicalDevice(m_SwapChain);
            CreateCommandPools();
            CreateCommandBuffers();
            m_SwapChain->Invalidate();
            CreateSyncObjects();
            s_InitializedLogicalContext = true;
        }
        else
        {
            m_SwapChain->Invalidate();
        }

        VEX_CORE_TRACE("Initialized context for window");
    }

    void VulkanContext::CreateContext()
    {
        VEX_RELEASE_ASSERT(!s_InitializedContext, "Context already exists");
        s_InitializedContext = true;

        std::cout << std::endl;
        VEX_CORE_TRACE("Initializing vulkan context");

        CreateInstance();
        SetupDebugMessenger();
        SelectPhysicalDevice();

        VEX_CORE_TRACE("Initialized global context");
    }

    void VulkanContext::DestroyContext()
    {
        s_LogicalDevice.waitIdle();

        s_TransferCommandBuffer = nullptr;
        s_TransferCommandPool = nullptr;
        s_TransferQueue = nullptr;
        s_TransferQueueIndex = u32_max;

        s_GraphicsCommandBuffers.clear();
        s_GraphicsCommandPool = nullptr;
        s_GraphicsQueue = nullptr;
        s_GraphicsQueueIndex = u32_max;

        s_LogicalDevice = nullptr;

        s_InitializedContext = false;
    }

    void VulkanContext::CreateInstance()
    {
        VEX_CORE_TRACE("Creating a vulkan instance");

        vk::ApplicationInfo appInfo;
        appInfo.pApplicationName = "Vexel Application";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Vexel";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = vk::ApiVersion13;

        std::vector<const char*> enabledExtensions = GetRequiredInstanceExtensions(s_Context.Context);
        std::vector<const char*> enabledLayers = GetRequiredInstanceLayers(s_Context.Context);

        vk::InstanceCreateInfo instanceInfo;
        instanceInfo.pApplicationInfo = &appInfo;
        instanceInfo.enabledExtensionCount = enabledExtensions.size();
        instanceInfo.ppEnabledExtensionNames = enabledExtensions.data();
        instanceInfo.enabledLayerCount = enabledLayers.size();
        instanceInfo.ppEnabledLayerNames = enabledLayers.data();

        s_Context.Instance = vk::raii::Instance{s_Context.Context, instanceInfo};
    }

    void VulkanContext::SetupDebugMessenger()
    {
        VEX_CORE_TRACE("Setting up debug messenger");

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);
        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT = {};
        debugUtilsMessengerCreateInfoEXT.messageSeverity = severityFlags;
        debugUtilsMessengerCreateInfoEXT.messageType = messageTypeFlags;
        debugUtilsMessengerCreateInfoEXT.pfnUserCallback = &VulkanDebugCallback;

        s_Context.DebugMessenger =
            vk::raii::DebugUtilsMessengerEXT{s_Context.Instance, debugUtilsMessengerCreateInfoEXT};
    }

    void VulkanContext::SelectPhysicalDevice()
    {
        VEX_CORE_TRACE("Selecting a vulkan physical device");

        std::vector<vk::raii::PhysicalDevice> availablePds = s_Context.Instance.enumeratePhysicalDevices();
        if (s_EnableInfoLogs)
        {
            VEX_CORE_INFO("Found {} vulkan physical devices:", availablePds.size());
            for (auto& pd : availablePds)
                VEX_CORE_INFO("\t{}", pd.getProperties().deviceName.data());
        }

        bool picked = false;
        for (auto& pd : availablePds)
        {
            vk::PhysicalDeviceProperties props = pd.getProperties();

            if (props.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
            {
                if (s_EnableInfoLogs)
                    VEX_CORE_INFO("{} is not a discrete GPU, skipping", props.deviceName.data());

                continue;
            }

            if (s_EnableInfoLogs)
                VEX_CORE_INFO("Found discrete GPU, picking: {}", props.deviceName.data());

            s_Context.PhysicalDevice = pd;
            picked = true;
            break;
        }

        if (!picked)
            s_Context.PhysicalDevice = availablePds.back();

        VEX_CORE_INFO(
            "Using physical device: {}", s_Context.PhysicalDevice.getProperties().deviceName.data());

        VEX_CORE_INFO("Available device extensions:");
        for (vk::ExtensionProperties& ext : s_Context.PhysicalDevice.enumerateDeviceExtensionProperties())
            VEX_CORE_INFO("\t{}", ext.extensionName.data());
    }

    void VulkanContext::CreateLogicalDevice(Ref<VulkanSwapChain> swapChain)
    {
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos = CreateQueue(swapChain);

        vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features,
            vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT,
            vk::PhysicalDeviceShaderObjectFeaturesEXT>
            featureChain{vk::PhysicalDeviceFeatures2{},
                vk::PhysicalDeviceVulkan11Features{}.setShaderDrawParameters(VK_TRUE),
                vk::PhysicalDeviceVulkan13Features{}.setDynamicRendering(VK_TRUE).setSynchronization2(
                    VK_TRUE),
                vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}.setExtendedDynamicState(VK_TRUE),
                vk::PhysicalDeviceShaderObjectFeaturesEXT{}.setShaderObject(VK_TRUE)};

        std::vector<const char*> requiredDeviceExtensions = {
            vk::KHRSwapchainExtensionName, "VK_EXT_shader_object"};

        vk::DeviceCreateInfo createInfo = {};
        createInfo.setPNext(&featureChain.get<vk::PhysicalDeviceFeatures2>())
            .setQueueCreateInfoCount(queueCreateInfos.size())
            .setPQueueCreateInfos(queueCreateInfos.data())
            .setEnabledExtensionCount(static_cast<u32>(requiredDeviceExtensions.size()))
            .setPpEnabledExtensionNames(requiredDeviceExtensions.data());

        s_LogicalDevice = vk::raii::Device{
            s_Context.PhysicalDevice,
            createInfo,
        };

        s_GraphicsQueue = vk::raii::Queue{s_LogicalDevice, s_GraphicsQueueIndex, 0};
        s_TransferQueue = vk::raii::Queue{s_LogicalDevice, s_TransferQueueIndex, 0};
    }

    std::vector<vk::DeviceQueueCreateInfo> VulkanContext::CreateQueue(Ref<VulkanSwapChain> swapChain)
    {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
            s_Context.PhysicalDevice.getQueueFamilyProperties();

        for (u32 i{}; i < queueFamilyProperties.size(); ++i)
        {
            if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
                s_Context.PhysicalDevice.getSurfaceSupportKHR(i, swapChain->GetSurface()) &&
                s_GraphicsQueueIndex == u32_max)
            {
                s_GraphicsQueueIndex = i;
                continue;
            }

            if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer &&
                s_TransferQueueIndex == u32_max)
            {
                s_TransferQueueIndex = i;
                continue;
            }
        }

        VEX_RELEASE_ASSERT(s_GraphicsQueueIndex != u32_max,
            "Failed to find a queue that supports both graphics and present");

        VEX_RELEASE_ASSERT(s_TransferQueueIndex != u32_max, "Failed to find a queue that supports transfer");

        float queuePriority = 0.5f;

        vk::DeviceQueueCreateInfo graphicsQueueCreateInfo = {};
        graphicsQueueCreateInfo.queueFamilyIndex = s_GraphicsQueueIndex;
        graphicsQueueCreateInfo.queueCount = 1;
        graphicsQueueCreateInfo.pQueuePriorities = &queuePriority;

        vk::DeviceQueueCreateInfo TransferQueueCreateInfo = {};
        TransferQueueCreateInfo.queueFamilyIndex = s_TransferQueueIndex;
        TransferQueueCreateInfo.queueCount = 1;
        TransferQueueCreateInfo.pQueuePriorities = &queuePriority;

        return {graphicsQueueCreateInfo, TransferQueueCreateInfo};
    }

    void VulkanContext::CreateCommandPools()
    {
        /* From Vulkan
         *
         *typedef struct VkCommandPoolCreateInfo {
         *    VkStructureType             sType;
         *    const void*                 pNext;
         *    VkCommandPoolCreateFlags    flags;
         *    uint32_t                    queueFamilyIndex;
         *} VkCommandPoolCreateInfo;
         */

        vk::CommandPoolCreateInfo poolInfo = {};
        poolInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;
        poolInfo.queueFamilyIndex = s_GraphicsQueueIndex;

        s_GraphicsCommandPool = vk::raii::CommandPool{s_LogicalDevice, poolInfo};

        poolInfo.queueFamilyIndex = s_TransferQueueIndex;
        s_TransferCommandPool = vk::raii::CommandPool{s_LogicalDevice, poolInfo};
    }

    void VulkanContext::CreateCommandBuffers()
    {
        vk::CommandBufferAllocateInfo allocInfo = {};
        allocInfo.commandPool = s_GraphicsCommandPool;
        allocInfo.level = vk::CommandBufferLevel::ePrimary;
        allocInfo.commandBufferCount = VulkanSwapChain::MaxFramesInFlight();

        s_GraphicsCommandBuffers = vk::raii::CommandBuffers{s_LogicalDevice, allocInfo};

        allocInfo.commandPool = s_TransferCommandPool;
        allocInfo.commandBufferCount = 1;

        s_TransferCommandBuffer = std::move(vk::raii::CommandBuffers{s_LogicalDevice, allocInfo}.front());
    }

    void VulkanContext::CreateSyncObjects()
    {
        VEX_RELEASE_ASSERT(VulkanRenderCommand::s_PresentCompleteSemaphores.empty() &&
                VulkanRenderCommand::s_RenderFinishedSemaphores.empty() &&
                VulkanRenderCommand::s_InFlightFences.empty(),
            "Sync objects already exist");

        vk::SemaphoreCreateInfo semaphoreInfo = {};
        vk::FenceCreateInfo fenceInfo = {};
        fenceInfo.flags = vk::FenceCreateFlagBits::eSignaled;

        for (u32 i{}; i < VulkanSwapChain::GetImageCount(); ++i)
        {
            VulkanRenderCommand::s_RenderFinishedSemaphores.emplace_back(s_LogicalDevice, semaphoreInfo);
        }

        for (u32 i{}; i < VulkanSwapChain::MaxFramesInFlight(); ++i)
        {
            VulkanRenderCommand::s_PresentCompleteSemaphores.emplace_back(s_LogicalDevice, semaphoreInfo);
            VulkanRenderCommand::s_InFlightFences.emplace_back(s_LogicalDevice, fenceInfo);
        }
    }

    namespace
    {
        std::vector<const char*> GetRequiredInstanceExtensions(vk::raii::Context& context)
        {
            std::vector<const char*> requiredExtensions;

            u32 glfwExtensionCount = 0;
            const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            for (u32 i{}; i < glfwExtensionCount; ++i)
            {
                VEX_RELEASE_ASSERT(std::ranges::any_of(context.enumerateInstanceExtensionProperties(),
                                       [ext = glfwExtensions[i]](const vk::ExtensionProperties& properties)
                { return strcmp(properties.extensionName, ext); }),
                    "Required GLFW extension not supported: {}", glfwExtensions[i]);
            }

            for (u32 i{}; i < glfwExtensionCount; ++i)
                requiredExtensions.emplace_back(std::move(glfwExtensions[i]));

            requiredExtensions.emplace_back(vk::EXTDebugUtilsExtensionName);

            if (s_EnableInfoLogs)
            {
                VEX_CORE_INFO("Available extensions:");
                for (vk::ExtensionProperties& ext : context.enumerateInstanceExtensionProperties())
                    VEX_CORE_INFO("\t{}", ext.extensionName.data());

                VEX_CORE_INFO("Required extensions:");
                for (const char* ext : requiredExtensions)
                    VEX_CORE_INFO("\t{}", ext);
            }

            return std::move(requiredExtensions);
        }

        std::vector<const char*> GetRequiredInstanceLayers(vk::raii::Context& context)
        {
            std::vector<const char*> requiredLayers;

            if (s_EnableValidationLayers)
                requiredLayers.assign(s_ValidationLayers.begin(), s_ValidationLayers.end());

            auto unsupportedLayerIt =
                std::ranges::find_if(requiredLayers, [&context](const char* requiredLayer)
            {
                return std::ranges::none_of(context.enumerateInstanceLayerProperties(),
                    [requiredLayer](const vk::LayerProperties& props)
                { return strcmp(props.layerName, requiredLayer) == 0; });
            });

            VEX_RELEASE_ASSERT(unsupportedLayerIt == requiredLayers.end(),
                "Vulkan driver does not support all required layers");

            if (s_EnableInfoLogs)
            {
                VEX_CORE_INFO("Required layers:");
                for (auto& layer : requiredLayers)
                    VEX_CORE_INFO("\t{}", layer);
            }

            return std::move(requiredLayers);
        }

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
        {
            switch (severity)
            {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                VEX_CORE_WARN("Validation layer: type {0} message: {1}",
                    vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)),
                    pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                VEX_CORE_ERROR("Validation layer: type {0} message: {1}",
                    vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)),
                    pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                break;
            }

            return vk::False;
        }
    } // namespace
} // namespace Vex
