#pragma once

namespace Vex
{
    /// @todo create VulkanLogicalDevice class
    class VulkanLogicalDevice
    {
      public:
        VulkanLogicalDevice(nullptr_t) {}

        static VulkanLogicalDevice Create() { return VulkanLogicalDevice(); }

      private:
        VulkanLogicalDevice() {}
    };
} // namespace Vex
