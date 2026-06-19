#pragma once

#include "Vexel/Core/Window.hpp"

namespace Vex
{
    enum class GraphicsAPIs
    {
        Vulkan
    };

    class RendererAPI
    {
      public:
        static void Init();
        static void Shutdown();

        static GraphicsAPIs API() { return s_GraphicsAPI; }

        static void SwapBuffers(Observer<Window> pWindow) {}

      private:
        inline static GraphicsAPIs s_GraphicsAPI = GraphicsAPIs::Vulkan;
    };
} // namespace Vex
