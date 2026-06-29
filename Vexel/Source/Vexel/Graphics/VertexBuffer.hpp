#pragma once

#include "Vexel/Renderer/RendererTypes.hpp"

#include "Vexel/Renderer/RendererAPI.hpp"

namespace Vex
{
    class VertexBuffer
    {
      public:
        virtual ~VertexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<VertexBuffer> Create();

      protected:
        VertexBuffer() = default;

      protected:
        RendererID m_RendererID = u32_max;
    };
} // namespace Vex
