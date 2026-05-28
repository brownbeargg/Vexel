#include "VulkanRendererAPI.hpp"

#include "Vexel/Utils.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    void VulkanRendererAPI::Init()
    {
        CreateInstance();
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

        uint32_t extensionCount = 0;
        const char** requiredInstanceExtensions = GetRequiredInstanceExtensions(extensionCount);

        std::vector availableExtensions = s_Context.enumerateInstanceExtensionProperties();
        
        std::cout << "Available extensions:";
        for (const auto& extension : availableExtensions)
            std::cout << "\t" << extension.extensionName << "\n";

        vk::InstanceCreateInfo createInfo{};
        createInfo.setPApplicationInfo(&appInfo)
            .setEnabledExtensionCount(extensionCount)
            .setPpEnabledExtensionNames(requiredInstanceExtensions);

        s_Instance = vk::raii::Instance(s_Context, createInfo);
    }

    const char** VulkanRendererAPI::GetRequiredInstanceExtensions(uint32_t& extensionCount)
    {
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&extensionCount);

        std::vector<vk::ExtensionProperties> extensionProperties =
            s_Context.enumerateInstanceExtensionProperties();

        // Check if all the glfw extensions are available

        for (uint32_t i{}; i < extensionCount; ++i)
            if (std::ranges::none_of(extensionProperties,
                    [glfwExtension = glfwExtensions[i]](const vk::ExtensionProperties& extensionProperty)
            { return strcmp(glfwExtension, extensionProperty.extensionName); }))
                VEX_RELEASE_ASSERT(false, "GLFW extension not supported: {}", glfwExtensions[i]);

        return glfwExtensions;
    }
} // namespace Vex
