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

        /// @todo make swapchain be able to have multiple windows
        m_SwapChain = VulkanSwapChain::Create(m_PhysicalDevice, Window::GetWindowInstances().at(0), surface);
    }
} // namespace Vex
