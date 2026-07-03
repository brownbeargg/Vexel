#pragma once

#include "Vexel/Core/Codes.hpp"
#include <glm/glm.hpp>

namespace Vex
{
    class Input
    {
      public:
        static bool IsKeyPressed(Key::Key key);
        static bool IsMouseButtonPressed(Mouse::Mouse button);
        static glm::vec2 GetMousePos();
    };
} // namespace Vex
