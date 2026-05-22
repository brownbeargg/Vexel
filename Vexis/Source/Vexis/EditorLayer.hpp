#pragma once

#include "Vexel/Core/Layer.hpp"

namespace Vex
{
    class EditorLayer final : public Layer
    {
      public:
        EditorLayer() : Layer("EditorLayer") {}
    };
} // namespace Vex
