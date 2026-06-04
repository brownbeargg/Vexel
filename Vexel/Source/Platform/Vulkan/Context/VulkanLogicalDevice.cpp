#include "VulkanLogicalDevice.hpp"

#include "Platform/Vulkan/Context/VulkanPhysicalDevice.hpp"

namespace Vex
{
    VulkanLogicalDevice::VulkanLogicalDevice(VulkanPhysicalDevice& pd, const QueueFamilyIndices& indices)
    {
        VEX_RELEASE_ASSERT(indices.IsComplete(), "Queue family indices list is NOT complete");

        std::set<uint32_t> uniqueFamilies = {indices.Graphics.value()};
        if (indices.Graphics.value() != indices.Present.value())
            uniqueFamilies.insert(indices.Present.value());

        float queuePriority = 1.0f;

        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        for (uint32_t family : uniqueFamilies)
        {
            vk::DeviceQueueCreateInfo createInfo{};
            createInfo.setQueueFamilyIndex(family).setPQueuePriorities(&queuePriority).setQueueCount(1);
            queueCreateInfos.push_back(createInfo);
        }

        std::vector<const char*> extensions{vk::KHRSwapchainExtensionName};
        vk::PhysicalDeviceFeatures features{};

        vk::DeviceCreateInfo createInfo{};
        createInfo.setQueueCreateInfoCount(queueCreateInfos.size())
            .setPQueueCreateInfos(queueCreateInfos.data())
            .setEnabledExtensionCount(extensions.size())
            .setPpEnabledExtensionNames(extensions.data())
            .setPEnabledFeatures(&features);

        m_pPhysicalDevice = &pd;
        m_LogicalDevice = m_pPhysicalDevice->CreateLogicalDevice(createInfo);

        m_GraphicsQueue = m_LogicalDevice.getQueue(indices.Graphics.value(), 0);
        m_PresentQueue = m_LogicalDevice.getQueue(indices.Present.value(), 0);
    }
} // namespace Vex
