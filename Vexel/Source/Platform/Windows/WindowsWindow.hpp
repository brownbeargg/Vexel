#pragma once

#include "Vexel/Core/Window.hpp"

struct GLFWwindow;

namespace Vex
{
    class WindowsWindow : public Window
    {
      public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void OnUpdate() override;

        uint32_t GetWidth() const override { return m_Props.Width; }
        uint32_t GetHeight() const override { return m_Props.Height; }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Props.VSync; }

        void* GetNativeWindow() const override { return m_Window; }

      private:
        WindowProps m_Props = {};

        GLFWwindow* m_Window = nullptr;
    };
} // namespace Vex
