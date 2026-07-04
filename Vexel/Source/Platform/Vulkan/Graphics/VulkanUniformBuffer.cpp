#include "VulkanUniformBuffer.hpp"

#include "Vexel/Graphics/UniformBufferObject.hpp"

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"
#include "Platform/Vulkan/Graphics/VulkanShader.hpp"

namespace Vex
{
    VulkanUniformBuffer::VulkanUniformBuffer(Ref<Shader> shader)
    {
        for (u32 i{}; i < VulkanSwapChain::MaxFramesInFlight(); ++i)
        {
            constexpr u32 bufferSize = sizeof(UniformBufferObject);

            m_Buffers.emplace_back(
                VulkanBuffer{VulkanBufferInput{bufferSize, vk::BufferUsageFlagBits::eUniformBuffer}});
            m_BuffersMapped.emplace_back(m_Buffers.back().Memory.mapMemory(0, bufferSize));
        }

        CreateDescriptorSets((VulkanShader*)shader.Get());
    }

    void VulkanUniformBuffer::Invalidate(const UniformBufferObject& ubo)
    {
        memcpy(m_BuffersMapped[VulkanContext::GetFrameIndex()], &ubo, sizeof(ubo));
    }

    void VulkanUniformBuffer::Bind(Ref<Shader> shader)
    {
        VulkanContext::GraphicsCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
            ((VulkanShader*)(shader.Get()))->GetPipelineLayout(), 0,
            {*m_DescriptorSets[VulkanContext::GetFrameIndex()]}, {});
    }

    void VulkanUniformBuffer::CreateDescriptorSets(VulkanShader* shader)
    {
        std::vector<vk::DescriptorSetLayout> layouts(
            VulkanSwapChain::MaxFramesInFlight(), shader->GetCameraDescriptorSetLayout());

        vk::DescriptorSetAllocateInfo allocInfo = {};
        allocInfo.descriptorPool = shader->GetCameraDescriptorPool();
        allocInfo.descriptorSetCount = VulkanSwapChain::MaxFramesInFlight();
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets = vk::raii::DescriptorSets{VulkanContext::LogicalDevice(), allocInfo};

        for (u32 i{}; i < VulkanSwapChain::MaxFramesInFlight(); ++i)
        {
            vk::DescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = m_Buffers[i].Buffer;
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            vk::WriteDescriptorSet descriptorWrite = {};
            descriptorWrite.dstSet = m_DescriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            VulkanContext::LogicalDevice().updateDescriptorSets(descriptorWrite, nullptr);
        }
    }
} // namespace Vex
