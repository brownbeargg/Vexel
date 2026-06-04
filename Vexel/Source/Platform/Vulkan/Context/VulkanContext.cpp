#include "VulkanContext.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
#if defined(NDEBUG)
    constexpr bool enableValidationLayers = false;
#else
    constexpr bool enableValidationLayers = true;
#endif

    std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VulkanContext::VulkanContext(Observer<Window> pWindow)
    {
        std::vector<const char*> requiredInstanceExtensions = GetRequiredInstanceExtensions();
        std::vector<const char*> requiredLayers = GetRequiredLayers();

        m_Instance = VulkanInstance::Create(*this, requiredInstanceExtensions, requiredLayers);

        /// @todo add support for multiple surfaces for multiple windows

        pWindow->CreateSurfaceVulkan(m_Instance.Get());

        m_Device =
            VulkanDevice::Create(m_Instance, {vk::KHRSwapchainExtensionName}, pWindow->GetSurfaceVulkan());
    }

    std::vector<const char*> VulkanContext::GetRequiredInstanceExtensions()
    {
        uint32_t glfwExtensionCount;
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<vk::ExtensionProperties> extensionProperties =
            m_Context.enumerateInstanceExtensionProperties();

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

    std::vector<const char*> VulkanContext::GetRequiredLayers()
    {
        std::vector<const char*> requiredLayers;
        if (enableValidationLayers)
            requiredLayers.assign(validationLayers.begin(), validationLayers.end());

        // Check if the required layers are supported by Vulkan
        std::vector<vk::LayerProperties> LayerProperties = m_Context.enumerateInstanceLayerProperties();

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
} // namespace Vex
