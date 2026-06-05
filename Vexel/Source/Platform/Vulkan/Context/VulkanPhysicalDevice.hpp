#pragma once

#include "Platform/Vulkan/Context/VulkanInstance.hpp"
#include "Platform/Vulkan/Context/VulkanLogicalDevice.hpp"
#include "Platform/Vulkan/Present/VulkanSurface.hpp"

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

        std::vector<vk::SurfaceFormatKHR> AvailableSurfaceFormats(VulkanSurface& surface)
        {
            return m_PhysicalDevice.getSurfaceFormatsKHR(surface.Get());
        }

        std::vector<vk::PresentModeKHR> AvailbleSurfacePresentModes(VulkanSurface& surface)
        {
            return m_PhysicalDevice.getSurfacePresentModesKHR(surface.Get());
        }

        vk::SurfaceCapabilitiesKHR GetSurfaceCapablities(VulkanSurface& surface)
        {
            return m_PhysicalDevice.getSurfaceCapabilitiesKHR(surface.Get());
        }

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
