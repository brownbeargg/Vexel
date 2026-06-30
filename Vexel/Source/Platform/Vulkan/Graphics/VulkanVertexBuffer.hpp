#pragma once

#include "Vexel/Graphics/VertexBuffer.hpp"
#include "Vexel/Graphics/VertexData.hpp"

#include "Platform/Vulkan/Graphics/VulkanBuffer.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanVertexBuffer final : public VertexBuffer
    {
      public:
        VulkanVertexBuffer(const std::vector<VertexData>& vertices);
        ~VulkanVertexBuffer() = default;

        void Bind() const override;
        void Unbind() const override;

      private:
        VulkanBuffer m_Buffer;
    };
} // namespace Vex
