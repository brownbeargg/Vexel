#pragma once

#include "Platform/Vulkan/Context/VulkanInstance.hpp"
#include "Platform/Vulkan/Context/VulkanLogicalDevice.hpp"
#include "Platform/Vulkan/Present/VulkanSurface.hpp"
#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanPhysicalDevice final
    {
      public:
        VulkanPhysicalDevice(nullptr_t) {}

        static VulkanPhysicalDevice Create(VulkanInstance& instance,
            const std::vector<const char*>& requiredDeviceExtensions, VulkanSurface& surface,
            vk::raii::PhysicalDevice* pd = nullptr)
        {
            return VulkanPhysicalDevice(instance, requiredDeviceExtensions, surface, pd);
        }

        vk::raii::Device CreateLogicalDevice(vk::DeviceCreateInfo& createInfo);
        VulkanLogicalDevice::QueueFamilyIndices& GetQueueFamilyIndices() { return m_QueueFamilyIndices; }

      private:
        VulkanPhysicalDevice(VulkanInstance& instance,
            const std::vector<const char*>& requiredDeviceExtensions, VulkanSurface& surface,
            vk::raii::PhysicalDevice* pd);

        void PickPhysicalDevice(
            const vk::raii::Instance& instance, const std::vector<const char*>& requiredDeviceExtensions);

      private:
        vk::raii::PhysicalDevice m_PhysicalDevice = nullptr;

        std::vector<vk::QueueFamilyProperties> m_QueueFamilies;
        VulkanLogicalDevice::QueueFamilyIndices m_QueueFamilyIndices;
    };
} // namespace Vex
