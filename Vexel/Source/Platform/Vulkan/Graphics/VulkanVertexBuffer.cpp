#include "VulkanVertexBuffer.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<VertexData>& vertices)
        : m_Buffer(VulkanBuffer(VulkanBufferInput{
              vertices.size() * sizeof(VertexData), vk::BufferUsageFlagBits::eVertexBuffer}))
    {
        void* data = m_Buffer.Memory.mapMemory(0, vertices.size() * sizeof(VertexData));
        memcpy(data, vertices.data(), vertices.size() * sizeof(VertexData));
        m_Buffer.Memory.unmapMemory();
    }

    void VulkanVertexBuffer::Bind() const
    {
        VulkanContext::QueryGraphicsCommandBuffer().bindVertexBuffers(
            0, {m_Buffer.Buffer}, vk::DeviceSize{0});
    }

    void VulkanVertexBuffer::Unbind() const {}
} // namespace Vex
