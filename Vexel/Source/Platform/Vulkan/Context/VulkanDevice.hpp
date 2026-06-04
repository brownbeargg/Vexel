#pragma once

#include "Platform/Vulkan/Context/VulkanLogicalDevice.hpp"
#include "Platform/Vulkan/Context/VulkanPhysicalDevice.hpp"

#include "Vexel/Core/Window.hpp"
#include "Vexel/Utils.hpp"

namespace Vex
{
    class VulkanDevice final
    {
      public:
        VulkanDevice(nullptr_t) {}

        static VulkanDevice Create(VulkanInstance& instance,
            const std::vector<const char*>& requiredExtensions, VulkanSurface& surface)
        {
            return VulkanDevice(instance, requiredExtensions, surface);
        }

        VulkanLogicalDevice& LogicalDevice() { return m_LogicalDevice; }
        VulkanPhysicalDevice& PhysicalDevice() { return m_PhysicalDevice; }

        const vk::raii::Queue& GraphicsQueue() const { return m_LogicalDevice.GraphicsQueue(); }
        vk::raii::Queue& GraphicsQueue() { return m_LogicalDevice.GraphicsQueue(); }

        const vk::raii::Queue& PresentQueue() const { return m_LogicalDevice.PresentQueue(); }
        vk::raii::Queue& PresentQueue() { return m_LogicalDevice.PresentQueue(); }

        void CreateSurface(VulkanInstance& instance, Observer<Window> window);

      private:
        /// @todo Create a way to have multiple surfaces, perhaps looping through each window
        VulkanDevice(VulkanInstance& instance, const std::vector<const char*>& requiredExtensions,
            VulkanSurface& surface);

      private:
        VulkanPhysicalDevice m_PhysicalDevice = nullptr;
        VulkanLogicalDevice m_LogicalDevice = nullptr;
    };
} // namespace Vex
