#pragma once

#include "Vexel/Graphics/VertexData.hpp"
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

        static Ref<VertexBuffer> Create(const std::vector<VertexData>& vertices);

      protected:
        VertexBuffer() = default;

      protected:
        RendererID m_RendererID = u32_max;
    };
} // namespace Vex
