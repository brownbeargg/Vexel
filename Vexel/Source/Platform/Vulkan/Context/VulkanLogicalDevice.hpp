#pragma once

#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanPhysicalDevice;

    class VulkanLogicalDevice final
    {
      public:
        struct QueueFamilyIndices
        {
            std::optional<uint32_t> Graphics;
            std::optional<uint32_t> Present;

            bool IsComplete() const { return Graphics.has_value() && Present.has_value(); }
        };

      public:
        VulkanLogicalDevice(nullptr_t) {}

        static VulkanLogicalDevice Create(VulkanPhysicalDevice& pd, const QueueFamilyIndices& indices)
        {
            return VulkanLogicalDevice(pd, indices);
        }

        const vk::raii::Device& Get() const { return m_LogicalDevice; }
        vk::raii::Device& Get() { return m_LogicalDevice; }

        const vk::raii::Queue& GraphicsQueue() const { return m_GraphicsQueue; }
        vk::raii::Queue& GraphicsQueue() { return m_GraphicsQueue; }

        const vk::raii::Queue& PresentQueue() const { return m_PresentQueue; }
        vk::raii::Queue& PresentQueue() { return m_PresentQueue; }

        const Observer<VulkanPhysicalDevice> GetPhysicalDevice() const { return m_pPhysicalDevice; }
        Observer<VulkanPhysicalDevice> GetPhysicalDevice() { return m_pPhysicalDevice; }

      private:
        VulkanLogicalDevice(VulkanPhysicalDevice& pd, const QueueFamilyIndices& indices);

      private:
        vk::raii::Device m_LogicalDevice = nullptr;

        vk::raii::Queue m_GraphicsQueue = nullptr;
        vk::raii::Queue m_PresentQueue = nullptr;

        Observer<VulkanPhysicalDevice> m_pPhysicalDevice = nullptr;
    };
} // namespace Vex
