#include "VulkanDevice.hpp"

#include "Vexel/Core/Application.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    VulkanDevice::VulkanDevice(
        VulkanInstance& instance, const std::vector<const char*>& requiredExtensions, VulkanSurface& surface)
    {
        m_PhysicalDevice = VulkanPhysicalDevice::Create(instance, requiredExtensions, surface);
        m_LogicalDevice =
            VulkanLogicalDevice::Create(m_PhysicalDevice, m_PhysicalDevice.GetQueueFamilyIndices());
    }
} // namespace Vex
