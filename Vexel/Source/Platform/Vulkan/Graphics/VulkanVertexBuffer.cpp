#include "VulkanVertexBuffer.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

namespace Vex
{
    VulkanVertexBuffer::VulkanVertexBuffer(const std::vector<VertexData>& vertices)
        : m_Buffer(VulkanBuffer(VulkanBufferInput{
              vertices.size() * sizeof(VertexData), vk::BufferUsageFlagBits::eVertexBuffer}))
    {
    }

    void VulkanVertexBuffer::Bind() const
    {
        VulkanContext::QueryGraphicsCommandBuffer().bindVertexBuffers(
            0, {m_Buffer.Buffer}, sizeof(VertexData));
    }

    void VulkanVertexBuffer::Unbind() const {}
} // namespace Vex
