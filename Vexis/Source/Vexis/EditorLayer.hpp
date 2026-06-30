#pragma once

#include "Vexel/Vexel.hpp"

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer() : Layer("EditorLayer")
        {
            m_Shader =
                Shader::Create(RootDirectory::Assets, "Shaders/Basic.vert.spv", "Shaders/Basic.frag.spv");

            std::vector<VertexData> vertices = {
                {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Bottom left
                {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom Right
                {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top right
                {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}} // Top left
            };

            m_VertexBuffer = VertexBuffer::Create(vertices);

            std::vector<u32> indices = {0, 1, 2, 0, 2, 3};
            m_IndexBuffer = IndexBuffer::Create(indices);
        }

        void OnUpdate(DeltaTime dt) override
        {
            m_Shader->Bind();
            RendererAPI::DrawIndexed(m_VertexBuffer, m_IndexBuffer);
        }

      private:
        Ref<Shader> m_Shader;
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
    };
} // namespace Vex
