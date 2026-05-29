#include "VulkanInstance.hpp"

#include "VulkanContext.hpp"

namespace Vex
{
#if defined(NDEBUG)
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

    VulkanInstance::VulkanInstance(VulkanContext& context, const std::vector<const char*>& requiredExtensions,
        const std::vector<const char*>& enabledLayers)
    {
        vk::ApplicationInfo appInfo{};
        appInfo.setPApplicationName("Vexel")
            .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
            .setPEngineName("No Engine")
            .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
            .setApiVersion(vk::ApiVersion13);

        vk::InstanceCreateInfo createInfo{};
        createInfo.setPApplicationInfo(&appInfo)
            .setEnabledExtensionCount(requiredExtensions.size())
            .setPpEnabledExtensionNames(requiredExtensions.data())
            .setEnabledLayerCount(enabledLayers.size())
            .setPpEnabledLayerNames(enabledLayers.data());

        m_Instance = vk::raii::Instance(context.Context(), createInfo);

        SetupDebugMessenger();
    }

    void VulkanInstance::SetupDebugMessenger()
    {
        if (!enableValidationLayers)
            return;

        vk::DebugUtilsMessageSeverityFlagsEXT severityFlags(
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
            vk::DebugUtilsMessageSeverityFlagBitsEXT::eError);

        vk::DebugUtilsMessageTypeFlagsEXT messageTypeFlags(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance |
            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation);

        vk::DebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfoEXT{};
        debugUtilsMessengerCreateInfoEXT.setMessageSeverity(severityFlags)
            .setMessageType(messageTypeFlags)
            .setPfnUserCallback(DebugCallback);

        m_DebugMessenger = m_Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanInstance::DebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity, VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
    {
        switch (severity)
        {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            VEX_CORE_WARN("Validation layer: type {0} message: {1}",
                vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)), pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            std::cout << pCallbackData->pMessage << "\n";
            VEX_CORE_ERROR("Validation layer: type {0} message: {1}",
                vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)), pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            break;
        }

        return vk::False;
    }
} // namespace Vex
