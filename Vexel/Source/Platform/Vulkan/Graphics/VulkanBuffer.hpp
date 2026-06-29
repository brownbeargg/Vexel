#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    struct VulkanBufferInput
    {
        u32 Size = 0;
        vk::BufferUsageFlags Usage;
        vk::raii::Device& LogicalDevice;
        vk::PhysicalDevice PhysicalDevice;
    };

    class VulkanBuffer
    {
      public:
        vk::raii::Buffer Buffer = nullptr;
        vk::raii::DeviceMemory Memory = nullptr;

      public:
        VulkanBuffer();
        VulkanBuffer(const VulkanBufferInput& input);

      private:
        u32 FindMemoryTypeIndex(
            vk::PhysicalDevice pd, u32 supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties);

        // TODO: Allocate staging buffer first
        void Allocate(const VulkanBufferInput& input);
    };
} // namespace Vex
