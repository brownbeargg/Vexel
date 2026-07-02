#pragma once

namespace Vex
{
    class ImGuiContext
    {
      public:
        static void CreateContext();
        static void DestroyContext();

        static void BeginFrame();
        static void EndFrame();
    };
} // namespace Vex
