#pragma once

#include "Vexel/Vexel.hpp"

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer() : Layer("EditorLayer")
        {
            m_QuadUniformBuffer = UniformBuffer::Create();
            m_TriangleUniformBuffer = UniformBuffer::Create();

            m_Shader = Shader::Create(RootDirectory::Assets, "Shaders/Basic.vert.spv",
                "Shaders/Basic.frag.spv", m_QuadUniformBuffer);
            std::vector<VertexData> quadVertices = {
                {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Bottom left
                {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom Right
                {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top right
                {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}} // Top left
            };

            std::vector<u32> quadIndices = {0, 1, 2, 0, 2, 3};

            m_QuadVertexBuffer = VertexBuffer::Create(quadVertices);
            m_QuadIndexBuffer = IndexBuffer::Create(quadIndices);

            std::vector<VertexData> triangleVertices = {
                {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Bottom left
                {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom Right
                {{0.0f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top
            };

            std::vector<u32> triangleIndices = {0, 1, 2};

            m_TriangleVertexBuffer = VertexBuffer::Create(triangleVertices);
            m_TriangleIndexBuffer = IndexBuffer::Create(triangleIndices);

            m_QuadMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
            m_TriangleMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        }

        void OnUpdate(DeltaTime dt) override
        {
            m_QuadMVP.Model =
                glm::rotate(m_QuadMVP.Model, glm::radians(10 * dt.Sec()), glm::vec3{0.0f, 0.0f, 1.0f});
            m_QuadUniformBuffer->Invalidate(m_QuadMVP);
            m_QuadUniformBuffer->Bind(m_Shader);

            m_Shader->Bind();
            RendererAPI::DrawIndexed(m_QuadVertexBuffer, m_QuadIndexBuffer);

            if (m_TriangleIsGrowing)
            {
                m_TriangleScale += m_TriangleSpeed * dt;

                if (m_TriangleScale >= 1.2f)
                    m_TriangleIsGrowing = false;
            }
            else
            {
                m_TriangleScale -= m_TriangleSpeed * dt;

                if (m_TriangleScale <= 0.3f)
                    m_TriangleIsGrowing = true;
            }

            m_TriangleMVP.Model = glm::scale(m_TriangleMVP.Model, glm::vec3(m_TriangleScale));

            m_TriangleUniformBuffer->Invalidate(m_TriangleMVP);
            m_TriangleUniformBuffer->Bind(m_Shader);

            m_TriangleMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
            m_Shader->Bind();
            RendererAPI::DrawIndexed(m_TriangleVertexBuffer, m_TriangleIndexBuffer);
        }

      private:
        Ref<Shader> m_Shader;

        Ref<UniformBuffer> m_QuadUniformBuffer;
        Ref<VertexBuffer> m_QuadVertexBuffer;
        Ref<IndexBuffer> m_QuadIndexBuffer;
        UniformBufferObject m_QuadMVP;

        Ref<UniformBuffer> m_TriangleUniformBuffer;
        Ref<VertexBuffer> m_TriangleVertexBuffer;
        Ref<IndexBuffer> m_TriangleIndexBuffer;
        UniformBufferObject m_TriangleMVP;

        float m_TriangleScale = 1;
        float m_TriangleSpeed = 0.3f;
        bool m_TriangleIsGrowing = true;
    };
} // namespace Vex
