#pragma once

namespace Vex
{
    class IndexBuffer
    {
      public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        u32 GetIndexCount() { return m_IndexCount; }

        static Ref<IndexBuffer> Create(const std::vector<u32>& indices);

      protected:
        IndexBuffer(u32 indexCount) : m_IndexCount(indexCount) {}

      protected:
        u32 m_IndexCount = 0;
    };
} // namespace Vex
