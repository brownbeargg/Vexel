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
        }

        void OnUpdate(DeltaTime dt) override {}

      private:
        Ref<VertexBuffer> m_VertexBuffer;
        Ref<Shader> m_Shader;
    };
} // namespace Vex
