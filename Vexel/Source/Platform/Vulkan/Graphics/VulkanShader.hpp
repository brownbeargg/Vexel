#include "Vexel/Graphics/Shader.hpp"

#include <vulkan/vulkan_raii.hpp>

namespace Vex
{
    class VulkanShader final : public Shader
    {
      public:
        VulkanShader(RootDirectory root, const std::filesystem::path& vertexPath,
            const std::filesystem::path& fragmentPath);

        void Bind() const override {}
        void Unbind() const override {}

      private:
        vk::raii::ShaderEXT m_VertexShader = nullptr;
        vk::raii::ShaderEXT m_FragmentShader = nullptr;
    };
} // namespace Vex
