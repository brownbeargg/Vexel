#include "VulkanBuffer.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

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

        Buffer = VulkanContext::QueryLogicalDevice().createBuffer(bufferInfo);

        Allocate(input);
    }

    u32 VulkanBuffer::FindMemoryTypeIndex(
        u32 supportedMemoryIndices, vk::MemoryPropertyFlags requestedProperties)
    {
        vk::raii::PhysicalDevice& pd = VulkanContext::QueryPhysicalDevice();
        vk::PhysicalDeviceMemoryProperties memoryProperties = pd.getMemoryProperties();
        for (u32 i{}; i < memoryProperties.memoryTypeCount; ++i)
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

        u32 memTypeIndex = FindMemoryTypeIndex(memoryRequirements.memoryTypeBits,
            vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent);

        VEX_RELEASE_ASSERT(memTypeIndex != u32_max, "Failed to find suitable memory type index");

        vk::MemoryAllocateInfo allocInfo = {};
        allocInfo.allocationSize = memoryRequirements.size;
        allocInfo.memoryTypeIndex = memTypeIndex;
        Memory = VulkanContext::QueryLogicalDevice().allocateMemory(allocInfo);
        Buffer.bindMemory(Memory, 0);
    }
} // namespace Vex
