#pragma once

#include "Platform/Vulkan/Graphics/VulkanBuffer.hpp"
#include "Vexel/Graphics/UniformBuffer.hpp"

namespace Vex
{
    class VulkanShader;

    class VulkanUniformBuffer final : public UniformBuffer
    {
      public:
        VulkanUniformBuffer(Ref<Shader> shader);

        void Invalidate(const UniformBufferObject& ubo) override;

        void Bind(Ref<Shader> shader) override;

      private:
        void CreateDescriptorSets(VulkanShader* shader);

      private:
        std::vector<VulkanBuffer> m_Buffers;
        std::vector<void*> m_BuffersMapped;

        std::vector<vk::raii::DescriptorSet> m_DescriptorSets;
    };
} // namespace Vex
