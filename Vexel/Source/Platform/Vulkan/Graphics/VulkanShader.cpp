#include "VulkanShader.hpp"

#include "Platform/Vulkan/Context/VulkanContext.hpp"

#include "Platform/Vulkan/Graphics/VulkanVertexData.hpp"

namespace Vex
{
    // TODO: Move to VexelUtils
    static std::vector<char> ReadFile(RootDirectory root, const std::filesystem::path& path)
    {
        std::ifstream file(FileSystem::Resolve(root, path), std::ios::ate | std::ios::binary);
        VEX_RELEASE_ASSERT(
            file.is_open(), "Failed to open file: {}", FileSystem::Resolve(root, path).string());

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return std::move(buffer);
    }

    static std::vector<char> ReadSpirv(RootDirectory root, const std::filesystem::path& path)
    {
        std::vector<char> bytes = ReadFile(root, path);

        VEX_RELEASE_ASSERT(bytes.size() % 4 == 0, "SPIR-V size is not multiple of 4");

        return bytes;
    }

    VulkanShader::VulkanShader(RootDirectory root, const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath)
    {
        vk::raii::ShaderModule vertModule = CreateShaderModule(ReadSpirv(root, vertexPath));
        vk::raii::ShaderModule fragModule = CreateShaderModule(ReadSpirv(root, fragmentPath));

        vk::PipelineShaderStageCreateInfo vertStage = {};
        vertStage.stage = vk::ShaderStageFlagBits::eVertex;
        vertStage.module = vertModule;
        vertStage.pName = "main";

        vk::PipelineShaderStageCreateInfo fragStage = {};
        fragStage.stage = vk::ShaderStageFlagBits::eFragment;
        fragStage.module = fragModule;
        fragStage.pName = "main";

        std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages = {vertStage, fragStage};

        /* From Vulkan
         *
         *typedef struct VkGraphicsPipelineCreateInfo {
         *    VkStructureType                                  sType;
         *    const void*                                      pNext;
         *    VkPipelineCreateFlags                            flags;
         *    uint32_t                                         stageCount;
         *    const VkPipelineShaderStageCreateInfo*           pStages;
         *    const VkPipelineVertexInputStateCreateInfo*      pVertexInputState;
         *    const VkPipelineInputAssemblyStateCreateInfo*    pInputAssemblyState;
         *    const VkPipelineTessellationStateCreateInfo*     pTessellationState;
         *    const VkPipelineViewportStateCreateInfo*         pViewportState;
         *    const VkPipelineRasterizationStateCreateInfo*    pRasterizationState;
         *    const VkPipelineMultisampleStateCreateInfo*      pMultisampleState;
         *    const VkPipelineDepthStencilStateCreateInfo*     pDepthStencilState;
         *    const VkPipelineColorBlendStateCreateInfo*       pColorBlendState;
         *    const VkPipelineDynamicStateCreateInfo*          pDynamicState;
         *    VkPipelineLayout                                 layout;
         *    VkRenderPass                                     renderPass;
         *    uint32_t                                         subpass;
         *    VkPipeline                                       basePipelineHandle;
         *    int32_t                                          basePipelineIndex;
         *} VkGraphicsPipelineCreateInfo;
         */

        vk::VertexInputBindingDescription bindingDesc = VulkanVertexData::GetVertexBindingDescription();
        std::array<vk::VertexInputAttributeDescription, 2> attributeDescriptions =
            VulkanVertexData::GetVertexAttributeDescriptions();

        vk::PipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDesc;
        vertexInputInfo.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

        std::vector<vk::DynamicState> dynamicStates = {
            vk::DynamicState::eViewport, vk::DynamicState::eScissor};

        vk::PipelineDynamicStateCreateInfo dynamicState = {};
        dynamicState.dynamicStateCount = dynamicStates.size();
        dynamicState.pDynamicStates = dynamicStates.data();

        vk::PipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.topology = vk::PrimitiveTopology::eTriangleList;

        vk::Extent2D extent = VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetExtent();

        vk::Viewport viewport = vk::Viewport{
            0.0f, 0.0f, static_cast<float>(extent.width), static_cast<float>(extent.height), 0.0f, 1.0f};

        vk::Rect2D scissor = {vk::Offset2D{0, 0}, extent};

        vk::PipelineViewportStateCreateInfo viewportState = {};
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        vk::PipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.depthClampEnable = vk::False;
        rasterizer.rasterizerDiscardEnable = vk::False;
        rasterizer.polygonMode = vk::PolygonMode::eFill;

        // TODO: set culling
        rasterizer.cullMode = vk::CullModeFlagBits::eNone;
        rasterizer.frontFace = vk::FrontFace::eCounterClockwise;
        rasterizer.depthBiasEnable = vk::False;
        rasterizer.lineWidth = 1.0f;

        vk::PipelineMultisampleStateCreateInfo multiSampling = {};
        multiSampling.rasterizationSamples = vk::SampleCountFlagBits::e1;
        multiSampling.sampleShadingEnable = vk::False;

        vk::PipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.blendEnable = vk::True;
        colorBlendAttachment.srcColorBlendFactor = vk::BlendFactor::eSrcAlpha;
        colorBlendAttachment.dstColorBlendFactor = vk::BlendFactor::eOneMinusSrcAlpha;
        colorBlendAttachment.colorBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachment.srcAlphaBlendFactor = vk::BlendFactor::eOne;
        colorBlendAttachment.dstAlphaBlendFactor = vk::BlendFactor::eZero;
        colorBlendAttachment.alphaBlendOp = vk::BlendOp::eAdd;
        colorBlendAttachment.colorWriteMask = vk::ColorComponentFlagBits::eR |
            vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;

        vk::PipelineColorBlendStateCreateInfo colorBlending = {};
        colorBlending.logicOpEnable = vk::False;
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;

        vk::PipelineLayoutCreateInfo pipelineLayoutCreateInfo = {};
        pipelineLayoutCreateInfo.setLayoutCount = 0;
        pipelineLayoutCreateInfo.pushConstantRangeCount = 0;

        m_PipelineLayout =
            vk::raii::PipelineLayout{VulkanContext::QueryLogicalDevice(), pipelineLayoutCreateInfo};

        vk::PipelineRenderingCreateInfo renderingInfo = {};
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachmentFormats =
            &VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetFormat().format;
        renderingInfo.colorAttachmentCount = 1;
        renderingInfo.pColorAttachmentFormats =
            &VulkanContext::GetCurrentVulkanContext()->GetSwapChain()->GetFormat().format;

        vk::GraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.stageCount = shaderStages.size();
        pipelineInfo.pStages = shaderStages.data();
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multiSampling;
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;
        pipelineInfo.layout = m_PipelineLayout;
        pipelineInfo.renderPass = nullptr;
        pipelineInfo.pNext = &renderingInfo;

        m_Pipeline = vk::raii::Pipeline{VulkanContext::QueryLogicalDevice(), nullptr, pipelineInfo};
    }

    void VulkanShader::Bind() const
    {
        VulkanContext::QueryGraphicsCommandBuffer().bindPipeline(
            vk::PipelineBindPoint::eGraphics, m_Pipeline);
    }

    void VulkanShader::Unbind() const {}

    vk::raii::ShaderModule VulkanShader::CreateShaderModule(const std::vector<char>& code) const
    {
        vk::ShaderModuleCreateInfo createInfo = {};
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const u32*>(code.data());

        return vk::raii::ShaderModule{VulkanContext::QueryLogicalDevice(), createInfo};
    }
} // namespace Vex
