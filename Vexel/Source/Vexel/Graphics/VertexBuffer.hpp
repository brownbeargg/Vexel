#pragma once

#include "Vexel/Graphics/VertexData.hpp"

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
    };
} // namespace Vex
