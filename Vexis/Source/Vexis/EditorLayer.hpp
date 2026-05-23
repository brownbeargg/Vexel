#pragma once

#include "Vexel/Core/Layer.hpp"

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer() : Layer("EditorLayer") {}

        void OnFixedUpdate(TimeStep ts) override { VEX_WARN("OnFixedUpdate with deltatime: {}", ts.Sec()); }
    };
} // namespace Vex
