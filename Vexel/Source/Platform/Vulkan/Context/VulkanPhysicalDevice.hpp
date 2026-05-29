#pragma once

#include "Platform/Vulkan/Context/VulkanInstance.hpp"
#include "Vexel/Utils.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanPhysicalDevice
    {
      public:
        VulkanPhysicalDevice(nullptr_t) {}

        static VulkanPhysicalDevice Create(VulkanInstance& instance,
            const std::vector<const char*>& requiredDeviceExtensions, vk::raii::PhysicalDevice* pd = nullptr)
        {
            return VulkanPhysicalDevice(instance, requiredDeviceExtensions, pd);
        }

      private:
        VulkanPhysicalDevice(VulkanInstance& instance,
            const std::vector<const char*>& requiredDeviceExtensions, vk::raii::PhysicalDevice* pd = nullptr);

        void PickPhysicalDevice(
            const vk::raii::Instance& instance, const std::vector<const char*>& requiredDeviceExtensions);

      private:
        vk::raii::PhysicalDevice m_PhysicalDevice = nullptr;
    };
} // namespace Vex
