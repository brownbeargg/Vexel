#include "Shader.hpp"

#include "Platform/Vulkan/Graphics/VulkanShader.hpp"

namespace Vex
{
    Ref<Shader> Shader::Create(RootDirectory root, const std::filesystem::path& vertexPath,
        const std::filesystem::path& fragmentPath)
    {
        switch (RendererAPI::API())
        {
        case GraphicsAPIs::Vulkan:
            return new VulkanShader(root, vertexPath, fragmentPath);
        }

        VEX_RELEASE_ASSERT(false, "Unknown renderer API");
        return nullptr;
    }
} // namespace Vex
