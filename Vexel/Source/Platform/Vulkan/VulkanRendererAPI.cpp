#include "VulkanRendererAPI.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

#if defined(NDEBUG)
constexpr bool enableValidationLayers = false;
#else
constexpr bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers{"VK_LAYER_KHRONOS_validation"};

namespace Vex
{
    void VulkanRendererAPI::Init()
    {
        CreateInstance();
        SetupDebugMessenger();

        VulkanDevice::Init();
    }

    void VulkanRendererAPI::Shutdown() {}

    void VulkanRendererAPI::CreateInstance()
    {
        vk::ApplicationInfo appInfo{};
        appInfo.setPApplicationName("Vexel")
            .setApplicationVersion(VK_MAKE_VERSION(1, 0, 0))
            .setPEngineName("No Engine")
            .setEngineVersion(VK_MAKE_VERSION(1, 0, 0))
            .setApiVersion(vk::ApiVersion13);

        std::vector<const char*> enabledLayers = SetupRequiredLayers();

        std::vector<const char*> requiredInstanceExtensions = GetRequiredInstanceExtensions();
        requiredInstanceExtensions.shrink_to_fit();

        vk::InstanceCreateInfo createInfo{};
        createInfo.setPApplicationInfo(&appInfo)
            .setEnabledExtensionCount(requiredInstanceExtensions.size())
            .setPpEnabledExtensionNames(requiredInstanceExtensions.data())
            .setEnabledLayerCount(enabledLayers.size())
            .setPpEnabledLayerNames(enabledLayers.data());

        s_Instance = vk::raii::Instance(s_Context, createInfo);
    }

    void VulkanRendererAPI::SetupDebugMessenger()
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

        s_DebugMessenger = s_Instance.createDebugUtilsMessengerEXT(debugUtilsMessengerCreateInfoEXT);
    }

    std::vector<const char*> VulkanRendererAPI::GetRequiredInstanceExtensions()
    {
        uint32_t glfwExtensionCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<vk::ExtensionProperties> extensionProperties =
            s_Context.enumerateInstanceExtensionProperties();

        // Check if all the glfw extensions are available

        for (uint32_t i{}; i < glfwExtensionCount; ++i)
            if (std::ranges::none_of(extensionProperties,
                    [glfwExtension = glfwExtensions[i]](const vk::ExtensionProperties& extensionProperty)
            { return strcmp(glfwExtension, extensionProperty.extensionName) == 0; }))
                VEX_RELEASE_ASSERT(false, "GLFW extension not supported: {}", glfwExtensions[i]);

        std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
        if (enableValidationLayers)
            extensions.push_back(vk::EXTDebugUtilsExtensionName);

        return extensions;
    }

    std::vector<const char*> VulkanRendererAPI::SetupRequiredLayers()
    {
        std::vector<const char*> requiredLayers;
        if (enableValidationLayers)
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());

        // Check if the required layers are supported by Vulkan
        std::vector<vk::LayerProperties> LayerProperties = s_Context.enumerateInstanceLayerProperties();

        // this it will tell if all the layers are supported, if so it == requiredLayers.end()
        auto unsupportedLayerIt =
            std::ranges::find_if(requiredLayers, [&LayerProperties](const char* requiredLayer)
        {
            return std::ranges::none_of(LayerProperties,
                [requiredLayer](const vk::LayerProperties& layerProperty)
            { return strcmp(layerProperty.layerName, requiredLayer) == 0; });
        });

        VEX_RELEASE_ASSERT(unsupportedLayerIt == requiredLayers.end(), "Required layer not supported: {}",
            std::string(*unsupportedLayerIt));

        return requiredLayers;
    }

    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRendererAPI::DebugCallback(
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
            VEX_CORE_ERROR("Validation layer: type {0} message: {1}",
                vk::to_string(static_cast<vk::DebugUtilsMessageTypeFlagsEXT>(type)), pCallbackData->pMessage);
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
            break;
        }

        return vk::False;
    }
} // namespace Vex
