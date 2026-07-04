#include "Platform/Vulkan/Graphics/VulkanUniformBuffer.hpp"
#include "Vexel/Graphics/Shader.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanShader final : public Shader
    {
      public:
        VulkanShader(RootDirectory root, const std::filesystem::path& vertexPath,
            const std::filesystem::path& fragmentPath);

        void Bind() const override;

        vk::raii::PipelineLayout& GetPipelineLayout() { return m_PipelineLayout; }

        vk::raii::DescriptorSetLayout& GetCameraDescriptorSetLayout() { return m_CameraDescriptorSetLayout; }
        vk::raii::DescriptorPool& GetCameraDescriptorPool() { return m_CameraDescriptorPool; }

      private:
        vk::raii::ShaderModule CreateShaderModule(const std::vector<char>& code) const;

        void CreatePipeline(std::array<vk::PipelineShaderStageCreateInfo, 2> shaderStages);

        void CreateDescriptorPools();
        void CreateDescriptorSetLayouts();

      private:
        vk::raii::Pipeline m_Pipeline = nullptr;
        vk::raii::PipelineLayout m_PipelineLayout = nullptr;

        vk::raii::DescriptorPool m_CameraDescriptorPool = nullptr;
        vk::raii::DescriptorSetLayout m_CameraDescriptorSetLayout = nullptr;
    };
} // namespace Vex
