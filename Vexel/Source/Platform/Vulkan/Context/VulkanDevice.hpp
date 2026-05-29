#pragma once

#include "Platform/Vulkan/Context/VulkanLogicalDevice.hpp"
#include "Platform/Vulkan/Context/VulkanPhysicalDevice.hpp"

namespace Vex
{
    class VulkanDevice
    {
      public:
        VulkanDevice(nullptr_t) {}

        static VulkanDevice Create(VulkanInstance& instance)
        {
            return VulkanDevice(instance, {vk::KHRSwapchainExtensionName});
        }

      private:
        VulkanDevice(VulkanInstance& instance, const std::vector<const char*>& requiredExtensions);

      private:
        VulkanPhysicalDevice m_PhysicalDevice = nullptr;
        VulkanLogicalDevice m_LogicalDevice = nullptr;

        /// @todo Make VulkanDevice be able to find queue families
        /// @todo
    };
} // namespace Vex
