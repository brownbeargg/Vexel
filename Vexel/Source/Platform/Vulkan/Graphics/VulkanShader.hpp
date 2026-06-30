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
        void Unbind() const override;

      private:
        vk::raii::ShaderModule CreateShaderModule(const std::vector<char>& code) const;

      private:
        vk::raii::Pipeline m_Pipeline = nullptr;
        vk::raii::PipelineLayout m_PipelineLayout = nullptr;
    };
} // namespace Vex
