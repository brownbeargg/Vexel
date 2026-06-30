#pragma once

#include "Platform/Vulkan/Context/VulkanContext.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    struct VulkanBufferInput
    {
        size_t Size = 0;
        vk::BufferUsageFlags Usage;
    };

    class VulkanBuffer
    {
      public:
        vk::raii::Buffer Buffer = nullptr;
        vk::raii::DeviceMemory Memory = nullptr;

      public:
        VulkanBuffer() = default;
        VulkanBuffer(const VulkanBufferInput& input);
        ~VulkanBuffer() { VulkanContext::QueryLogicalDevice().waitIdle(); }

        VulkanBuffer(const VulkanBuffer& other) = delete;
        VulkanBuffer& operator=(const VulkanBuffer& other) = delete;

        VulkanBuffer(VulkanBuffer&& rhs)
        {
            if (&rhs == this)
                return;

            Buffer = std::move(rhs.Buffer);
            Memory = std::move(rhs.Memory);
        }

        VulkanBuffer& operator=(VulkanBuffer&& rhs)
        {
            if (&rhs == this)
                return *this;

            Buffer = std::move(rhs.Buffer);
            Memory = std::move(rhs.Memory);

            return *this;
        }
      private:
        u32 FindMemoryTypeIndex(u32 supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties);

        // TODO: Allocate staging buffer first
        void Allocate(const VulkanBufferInput& input);
    };
} // namespace Vex
