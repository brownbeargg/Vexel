#pragma once

#include "Vexel/Core/Layer.hpp"

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer(const std::string& message) : Layer("EditorLayer"), m_Message(message) {}

        void OnUpdate(DeltaTime dt) override { std::cout << m_Message << "\n"; }

      private:
        std::string m_Message;
    };
} // namespace Vex
