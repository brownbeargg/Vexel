#include "VulkanUniformBuffer.hpp"

#include "Vexel/Graphics/UniformBufferObject.hpp"

#include "Platform/Vulkan/Renderer/VulkanRenderCommand.hpp"
#include "Platform/Vulkan/Graphics/VulkanShader.hpp"

namespace Vex
{
    // TODO: Refactor
    VulkanUniformBuffer::VulkanUniformBuffer()
    {
        for (u32 i{}; i < VulkanSwapChain::MaxFramesInFlight(); ++i)
        {
            constexpr u32 bufferSize = sizeof(UniformBufferObject);

            m_Buffers.emplace_back(
                VulkanBuffer{VulkanBufferInput{bufferSize, vk::BufferUsageFlagBits::eUniformBuffer}});
            m_BuffersMapped.emplace_back(m_Buffers.back().Memory.mapMemory(0, bufferSize));
        }

        CreatePool();
        CreateSetLayout();
        CreateDescriptorSets();
    }

    void VulkanUniformBuffer::Invalidate(const UniformBufferObject& ubo)
    {
        memcpy(m_BuffersMapped[VulkanContext::GetFrameIndex()], &ubo, sizeof(ubo));
    }

    void VulkanUniformBuffer::Bind(Ref<Shader> shader)
    {
        // TODO: Do not hardcode index

        VulkanContext::QueryGraphicsCommandBuffer().bindDescriptorSets(vk::PipelineBindPoint::eGraphics,
            ((VulkanShader*)(shader.Get()))->GetPipelineLayout(), 0,
            {*m_DescriptorSets[VulkanContext::GetFrameIndex()]}, {});
    }

    void VulkanUniformBuffer::CreatePool()
    {
        vk::DescriptorPoolSize poolSize = {};
        poolSize.descriptorCount = VulkanSwapChain::MaxFramesInFlight();
        poolSize.type = vk::DescriptorType::eUniformBuffer;

        // TODO: Create 1 pool for multiple of these objects
        vk::DescriptorPoolCreateInfo poolInfo = {};
        poolInfo.poolSizeCount = 1;
        poolInfo.pPoolSizes = &poolSize;
        poolInfo.maxSets = VulkanSwapChain::MaxFramesInFlight();
        poolInfo.flags = vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet;

        m_DescriptorPool = vk::raii::DescriptorPool{VulkanContext::QueryLogicalDevice(), poolInfo};
    }

    void VulkanUniformBuffer::CreateSetLayout()
    {
        vk::DescriptorSetLayoutBinding layoutBindings = {};

        // TODO: make different bindings for different UBO types
        layoutBindings.binding = 0;
        layoutBindings.descriptorType = vk::DescriptorType::eUniformBuffer;
        // TODO: Multiple descriptors
        layoutBindings.descriptorCount = 1;
        layoutBindings.stageFlags = vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment;
        layoutBindings.pImmutableSamplers = nullptr;

        vk::DescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.bindingCount = 1;
        layoutInfo.pBindings = &layoutBindings;

        m_DescriptorSetLayout = {VulkanContext::QueryLogicalDevice(), layoutInfo};
    }

    void VulkanUniformBuffer::CreateDescriptorSets()
    {
        std::vector<vk::DescriptorSetLayout> layouts(
            VulkanSwapChain::MaxFramesInFlight(), *m_DescriptorSetLayout);

        vk::DescriptorSetAllocateInfo allocInfo = {};
        allocInfo.descriptorPool = *m_DescriptorPool;
        allocInfo.descriptorSetCount = VulkanSwapChain::MaxFramesInFlight();
        allocInfo.pSetLayouts = layouts.data();

        m_DescriptorSets = vk::raii::DescriptorSets{VulkanContext::QueryLogicalDevice(), allocInfo};

        for (u32 i{}; i < VulkanSwapChain::MaxFramesInFlight(); ++i)
        {
            vk::DescriptorBufferInfo bufferInfo = {};
            bufferInfo.buffer = m_Buffers[i].Buffer;
            bufferInfo.offset = 0;
            // TODO: multiple UBO types
            bufferInfo.range = sizeof(UniformBufferObject);

            vk::WriteDescriptorSet descriptorWrite = {};
            descriptorWrite.dstSet = m_DescriptorSets[i];
            descriptorWrite.dstBinding = 0;
            descriptorWrite.dstArrayElement = 0;
            descriptorWrite.descriptorType = vk::DescriptorType::eUniformBuffer;
            descriptorWrite.descriptorCount = 1;
            descriptorWrite.pBufferInfo = &bufferInfo;

            VulkanContext::QueryLogicalDevice().updateDescriptorSets(descriptorWrite, nullptr);
        }
    }
} // namespace Vex
