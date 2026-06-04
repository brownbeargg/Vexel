#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanContext;

    class VulkanInstance final
    {
      public:
        VulkanInstance() = default;
        VulkanInstance(nullptr_t) {}

        static VulkanInstance Create(VulkanContext& context,
            const std::vector<const char*>& requiredExtensions, std::vector<const char*> enabledLayers)
        {
            return VulkanInstance(context, requiredExtensions, enabledLayers);
        }

        const vk::raii::Instance& Get() const { return m_Instance; }
        vk::raii::Instance& Get() { return m_Instance; }

        // --------------------------------------------------------------------------------
        // Ignore, this is the debug callback for vulkan
        // --------------------------------------------------------------------------------

        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
            VkDebugUtilsMessageTypeFlagsEXT type, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData);

      private:
        VulkanInstance(VulkanContext& context, const std::vector<const char*>& requiredExtensions,
            const std::vector<const char*>& enabledLayers);

        void SetupDebugMessenger();

      private:
        vk::raii::Instance m_Instance = nullptr;
        vk::raii::DebugUtilsMessengerEXT m_DebugMessenger = nullptr;
    };
} // namespace Vex
