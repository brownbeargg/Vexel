#pragma once

#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    class UniformBuffer;

    class Shader
    {
      public:
        virtual ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<Shader> Create(RootDirectory root, const std::filesystem::path& vertexPath,
            const std::filesystem::path& fragmentPath, Ref<UniformBuffer> uniformBuffer);

      protected:
        Shader() = default;
    };
} // namespace Vex
