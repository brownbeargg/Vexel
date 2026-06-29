#include "VulkanBuffer.hpp"

namespace Vex
{
    VulkanBuffer::VulkanBuffer(const VulkanBufferInput& input)
    {
        /* From Vulkan
         *
         * typedef struct VkBufferCreateInfo {
         *    VkStructureType        sType;
         *    const void*            pNext;
         *    VkBufferCreateFlags    flags;
         *    VkDeviceSize           size;
         *    VkBufferUsageFlags     usage;
         *    VkSharingMode          sharingMode;
         *    uint32_t               queueFamilyIndexCount;
         *    const uint32_t*        pQueueFamilyIndices;
         *} VkBufferCreateInfo;
         */

        vk::BufferCreateInfo bufferInfo;
        bufferInfo.flags = vk::BufferCreateFlags{};
        bufferInfo.size = input.Size;
        bufferInfo.usage = input.Usage;
        bufferInfo.sharingMode = vk::SharingMode::eExclusive;

        Buffer = input.LogicalDevice.createBuffer(bufferInfo);
        Allocate(input);
    }

    u32 VulkanBuffer::FindMemoryTypeIndex(
        vk::PhysicalDevice pd, u32 supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties)
    {
        vk::PhysicalDeviceMemoryProperties memoryProperties = pd.getMemoryProperties();
        for (u32 i{}; i < memoryProperties; ++i)
        {
            bool supported = supportedMemoryIndices & VEX_BIT(i);
            bool sufficient =
                (memoryProperties.memoryTypes[i].propertyFlags & requestedProperties) == requestedProperties;

            if (supported && sufficient)
                return i;
        }

        return u32_max;
    }

    void VulkanBuffer::Allocate(const VulkanBufferInput& input)
    {
        vk::MemoryRequirements memoryRequirements = Buffer.getMemoryRequirements();

        vk::MemoryAllocateInfo allocInfo;
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex =
            FindMemoryTypeIndex(input.PhysicalDevice, memoryRequirements.memoryTypeBits,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

        Memory = input.LogicalDevice.allocateMemory(allocInfo);
        Buffer.bindMemory(Memory, 0);
    }
} // namespace Vex
