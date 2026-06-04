#include "VulkanPhysicalDevice.hpp"
#include "Platform/Vulkan/Present/VulkanSurface.hpp"

namespace Vex
{
    VulkanPhysicalDevice::VulkanPhysicalDevice(VulkanInstance& instance,
        const std::vector<const char*>& requiredDeviceExtensions, VulkanSurface& surface,
        vk::raii::PhysicalDevice* pd)
    {
        if (!pd)
            PickPhysicalDevice(instance.Get(), requiredDeviceExtensions);
        else
            m_PhysicalDevice = *pd;

        m_QueueFamilies = m_QueueFamilies = m_PhysicalDevice.getQueueFamilyProperties();
        for (uint32_t i{}; i < m_QueueFamilies.size(); ++i)
        {
            const vk::QueueFamilyProperties& family = m_QueueFamilies[i];

            if (family.queueFlags & vk::QueueFlagBits::eGraphics)
                m_QueueFamilyIndices.Graphics = i;

            vk::Bool32 presentSupport = m_PhysicalDevice.getSurfaceSupportKHR(i, surface.Get());
            if (presentSupport)
                m_QueueFamilyIndices.Present = i;
        }
    }

    vk::raii::Device VulkanPhysicalDevice::CreateLogicalDevice(vk::DeviceCreateInfo& createInfo)
    {
        return m_PhysicalDevice.createDevice(createInfo);
    }

    void VulkanPhysicalDevice::PickPhysicalDevice(
        const vk::raii::Instance& instance, const std::vector<const char*>& requiredDeviceExtensions)
    {
        std::vector<vk::raii::PhysicalDevice> physicalDevices = instance.enumeratePhysicalDevices();
        VEX_RELEASE_ASSERT(
            !physicalDevices.empty(), "Failed to find any physical device (gpu) with Vulkan support");

        std::multimap<int, vk::raii::PhysicalDevice> candidates;

        for (const vk::raii::PhysicalDevice& pd : physicalDevices)
        {
            vk::PhysicalDeviceProperties pdProperties = pd.getProperties();
            vk::PhysicalDeviceFeatures pdFeatures = pd.getFeatures();

            uint32_t score = 0;

            if (pdProperties.apiVersion < vk::ApiVersion13)
                continue;

            // Supports graphics

            auto queueFamilies = pd.getQueueFamilyProperties();
            bool supportsGraphics =
                std::ranges::any_of(queueFamilies, [](const vk::QueueFamilyProperties& qfp)
            { return !!(qfp.queueFlags & vk::QueueFlagBits::eGraphics); });

            // Required extension support

            auto availableDeviceExtensions = pd.enumerateDeviceExtensionProperties();
            bool supportsAllRequiredExtensions = std::ranges::all_of(requiredDeviceExtensions,
                [&availableDeviceExtensions](auto const& requiredDeviceExtension)
            {
                return std::ranges::any_of(availableDeviceExtensions,
                    [requiredDeviceExtension](auto const& availableDeviceExtension)
                { return strcmp(availableDeviceExtension.extensionName, requiredDeviceExtension) == 0; });
            });

            // Required feature support

            auto features = pd.template getFeatures2<vk::PhysicalDeviceFeatures2,
                vk::PhysicalDeviceVulkan13Features, vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>();

            bool supportsRequiredFeatures =
                features.template get<vk::PhysicalDeviceVulkan13Features>().dynamicRendering &&
                features.template get<vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>()
                    .extendedDynamicState;

            if (pdProperties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
                score += 1000;

            score += pdProperties.limits.maxImageDimension2D;

            if (!pdFeatures.geometryShader)
                continue;

            candidates.insert({score, pd});
        }
        VEX_RELEASE_ASSERT(!candidates.empty() && candidates.rbegin()->first > 0,
            "Found physical device(s) with Vulkan support, but there wasn't one suitable");

        m_PhysicalDevice = candidates.rbegin()->second;

        VEX_CORE_INFO("Using GPU: {}", m_PhysicalDevice.getProperties().deviceName.data());
    }
} // namespace Vex
