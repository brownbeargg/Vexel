#include "VulkanContext.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    namespace
    {
        static constexpr bool s_EnableInfoLogs = true;

        static constexpr bool s_EnableValidationLayers = true;
        static std::vector<const char*> s_ValidationLayers = {"VK_LAYER_KHRONOS_validation"};

        std::vector<const char*> GetRequiredInstanceExtensions(vk::raii::Context& context);
        std::vector<const char*> GetRequiredInstanceLayers(vk::raii::Context& context);

        VKAPI_ATTR VkBool32 VKAPI_CALL VulkanDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);
    } // namespace

    void VulkanContext::Init()
    {
        m_SwapChain = Ref<VulkanSwapChain>::Create(
            s_Context.Instance, s_Context.PhysicalDevice, m_LogicalDevice, m_pWindow);

        CreateLogicalDevice();

        m_SwapChain->Invalidate();
    }

    void VulkanContext::CreateContext()
    {
        static bool s_InitializedContext = false;

        VEX_RELEASE_ASSERT(!s_InitializedContext, "Context already exists");
        s_InitializedContext = true;

        std::cout << std::endl;
        VEX_CORE_TRACE("Initializing vulkan context");

        CreateInstance();
        SetupDebugMessenger();
        SelectPhysicalDevice();
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
    }

    void VulkanContext::CreateLogicalDevice()
    {
        vk::DeviceQueueCreateInfo queueCreateInfo = CreateQueue();

        vk::StructureChain<vk::PhysicalDeviceFeatures2, vk::PhysicalDeviceVulkan11Features,
            vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
            featureChain{vk::PhysicalDeviceFeatures2{},
                vk::PhysicalDeviceVulkan11Features{}.setShaderDrawParameters(VK_TRUE),
                vk::PhysicalDeviceVulkan13Features{}.setDynamicRendering(VK_TRUE).setSynchronization2(
                    VK_TRUE),
                vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT{}.setExtendedDynamicState(VK_TRUE)};

        std::vector<const char*> requiredDeviceExtensions = {vk::KHRSwapchainExtensionName};

        vk::DeviceCreateInfo createInfo = {};
        createInfo.setPNext(&featureChain.get<vk::PhysicalDeviceFeatures2>())
            .setQueueCreateInfoCount(1)
            .setPQueueCreateInfos(&queueCreateInfo)
            .setEnabledExtensionCount(static_cast<u32>(requiredDeviceExtensions.size()))
            .setPpEnabledExtensionNames(requiredDeviceExtensions.data());

        m_LogicalDevice = vk::raii::Device{
            s_Context.PhysicalDevice,
            createInfo,
        };

        m_GraphicsQueue = vk::raii::Queue{m_LogicalDevice, m_GraphicsQueueIndex, 0};
    }

    vk::DeviceQueueCreateInfo VulkanContext::CreateQueue()
    {
        std::vector<vk::QueueFamilyProperties> queueFamilyProperties =
            s_Context.PhysicalDevice.getQueueFamilyProperties();

        for (u32 i{}; i < queueFamilyProperties.size(); ++i)
        {
            if ((queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eGraphics) &&
                s_Context.PhysicalDevice.getSurfaceSupportKHR(i, m_SwapChain->GetSurface()) &&
                m_GraphicsQueueIndex == u32_max)
            {
                m_GraphicsQueueIndex = i;
                continue;
            }

            if (queueFamilyProperties[i].queueFlags & vk::QueueFlagBits::eTransfer &&
                m_TransferQueueIndex == u32_max)
            {
                m_TransferQueueIndex = i;
                continue;
            }
        }

        VEX_RELEASE_ASSERT(m_GraphicsQueueIndex != u32_max,
            "Failed to find a queue that supports both graphics and present");

        VEX_RELEASE_ASSERT(m_TransferQueueIndex != u32_max, "Failed to find a queue that supports transfer");

        float queuePriority = 0.5f;

        vk::DeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.queueFamilyIndex = m_GraphicsQueueIndex;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        return std::move(queueCreateInfo);
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
