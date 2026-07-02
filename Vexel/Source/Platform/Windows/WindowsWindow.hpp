#pragma once

#include "Vexel/Core/Window.hpp"
#include "Vexel/Renderer/RendererContext.hpp"

#include <GLFW/glfw3.h>

namespace Vex
{
    class WindowsWindow : public Window
    {
      public:
        WindowsWindow(const WindowProps& props);
        ~WindowsWindow();

        void PollEvents() override;

        bool ShouldClose() const override;
        void Close() override;

        u32 GetWidth() const override { return m_Data.Width; }
        u32 GetHeight() const override { return m_Data.Height; }

        void GetFrameBufferSize(int* width, int* height) const override
        {
            glfwGetFramebufferSize(m_Window, width, height);
        }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override { return m_Data.VSync; }

        bool IsOpen() const override { return m_Data.Open; }
        bool IsFocused() const override { return m_Data.Focus; }
        bool IsMinimized() const override { return m_Data.Minimized; }

        void* GetNativeWindow() const override { return m_Window; }

        void SetEventCallbackFn(const EventCallbackFn& fn) override { m_Data.EventCallback = fn; }

        Ref<RendererContext> GetRendererContext() override { return m_RendererContext; }

      private:
        void SetEventCallbacks();

      private:
        struct WindowData
        {
            Observer<Window> Self = {};

            std::string Title;

            uint32_t Width, Height;
            int XPos, YPos;

            bool Focus;
            bool VSync;
            bool HasTitleBar;
            bool Resizable;

            bool Open = true;
            bool Minimized = false;

            EventCallbackFn EventCallback = nullptr;

            WindowData(const WindowProps& props)
                : Title(props.Title), Width(props.Width), Height(props.Height), XPos(props.XPos),
                  YPos(props.YPos), Focus(props.Focus), VSync(props.VSync), HasTitleBar(props.HasTitleBar)
            {
            }
        };

        WindowData m_Data;

        GLFWwindow* m_Window = nullptr;

        Ref<RendererContext> m_RendererContext = nullptr;
    };
} // namespace Vex
