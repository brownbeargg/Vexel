#pragma once

namespace Vex
{
    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static Ref<IndexBuffer> Create(const std::vector<u32>& indices);

      protected:
        IndexBuffer() = default;
    };
} // namespace Vex
