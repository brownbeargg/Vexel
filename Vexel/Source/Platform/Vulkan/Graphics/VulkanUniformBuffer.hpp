#pragma once

#include "Platform/Vulkan/Graphics/VulkanBuffer.hpp"
#include "Vexel/Graphics/UniformBuffer.hpp"

namespace Vex
{
    class VulkanUniformBuffer final : public UniformBuffer
    {
      public:
        VulkanUniformBuffer();

        void Invalidate(const UniformBufferObject& ubo) override;

        void Bind(Ref<Shader> shader) override;

        const vk::raii::DescriptorSetLayout& GetSetLayout() const { return m_DescriptorSetLayout; }

      private:
        std::vector<VulkanBuffer> m_Buffers;
        std::vector<void*> m_BuffersMapped;

        vk::raii::DescriptorSetLayout m_DescriptorSetLayout = nullptr;

        vk::raii::DescriptorPool m_DescriptorPool = nullptr;
        std::vector<vk::raii::DescriptorSet> m_DescriptorSets;
    };
} // namespace Vex
