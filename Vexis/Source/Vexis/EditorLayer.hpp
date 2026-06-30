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

            std::vector<VertexData> vertices;
            vertices.emplace_back(glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
            vertices.emplace_back(glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            vertices.emplace_back(glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

            m_VertexBuffer = VertexBuffer::Create(vertices);
        }

        void OnUpdate(DeltaTime dt) override
        {
            m_Shader->Bind();
            m_VertexBuffer->Bind();

            m_Shader->Unbind();
            m_VertexBuffer->Unbind();
        }

      private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<Shader> m_Shader;
    };
} // namespace Vex
