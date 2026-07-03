#pragma once

#include "Vexel/Vexel.hpp"

#include <imgui.h>

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer();

        void OnUpdate(DeltaTime dt) override;
        void OnImGuiRender() override;

      private:
        void HandleInput();

      private:
        float m_FPS = 0;

        Ref<Shader> m_Shader;

        Ref<UniformBuffer> m_QuadUniformBuffer;
        Ref<VertexBuffer> m_QuadVertexBuffer;
        Ref<IndexBuffer> m_QuadIndexBuffer;
        UniformBufferObject m_QuadMVP;

        Ref<UniformBuffer> m_TriangleUniformBuffer;
        Ref<VertexBuffer> m_TriangleVertexBuffer;
        Ref<IndexBuffer> m_TriangleIndexBuffer;
        UniformBufferObject m_TriangleMVP;

        Ref<PerspectiveCamera> m_Camera;
        glm::vec3 m_CameraRotation{90.0f, 1.0f, 0.0f};
        float m_CameraSpeed = 0.005f;
        glm::vec2 m_LastMousePos{0.0f, 0.0f};

        float m_TriangleDistance = 1;
        float m_TriangleSpeed = 0.8f;
        bool m_TriangleBacking = true;
    };
} // namespace Vex
