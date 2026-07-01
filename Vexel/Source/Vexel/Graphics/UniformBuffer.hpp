#pragma once

#include "Vexel/Graphics/Shader.hpp"

namespace Vex
{
    struct UniformBufferObject;

    class UniformBuffer
    {
      public:
        virtual ~UniformBuffer() = default;

        virtual void Invalidate(const UniformBufferObject& ubo) = 0;

        virtual void Bind(Ref<Shader> shader) = 0;

        static Ref<UniformBuffer> Create();

      protected:
        UniformBuffer() = default;
    };
} // namespace Vex
