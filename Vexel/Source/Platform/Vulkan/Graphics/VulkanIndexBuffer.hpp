#pragma once

#include "Platform/Vulkan/Graphics/VulkanBuffer.hpp"
#include "Vexel/Graphics/IndexBuffer.hpp"

namespace Vex
{
    class VulkanIndexBuffer final : public IndexBuffer
    {
      public:
        VulkanIndexBuffer(const std::vector<u32>& indices);
        ~VulkanIndexBuffer() = default;

        void Bind() const override;
        void Unbind() const override;

      private:
        VulkanBuffer m_Buffer;
    };
} // namespace Vex
