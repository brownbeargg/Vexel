#include "VulkanDevice.hpp"

namespace Vex
{
    VulkanDevice::VulkanDevice(VulkanInstance& instance, const std::vector<const char*>& requiredExtensions)
    {
        m_PhysicalDevice = VulkanPhysicalDevice::Create(instance, requiredExtensions);
        m_LogicalDevice = VulkanLogicalDevice::Create();
    }
} // namespace Vex
