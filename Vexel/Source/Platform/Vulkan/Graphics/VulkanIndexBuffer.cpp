#include "VulkanIndexBuffer.hpp"

namespace Vex
{
    VulkanIndexBuffer::VulkanIndexBuffer(const std::vector<u32>& indices)
        : IndexBuffer(static_cast<u32>(indices.size())),
          m_Buffer(VulkanBufferInput{indices.size() * sizeof(u32), vk::BufferUsageFlagBits::eIndexBuffer})
    {
        void* data = m_Buffer.Memory.mapMemory(0, indices.size() * sizeof(u32));
        memcpy(data, indices.data(), indices.size() * sizeof(u32));
        m_Buffer.Memory.unmapMemory();
    }

    void VulkanIndexBuffer::Bind() const
    {
        VulkanContext::GraphicsCommandBuffer().bindIndexBuffer(
            m_Buffer.Buffer, 0, vk::IndexType::eUint32);
    }
} // namespace Vex
