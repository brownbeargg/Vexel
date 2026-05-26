#pragma once

#include "Vexel/Utils.hpp"

namespace Vex
{
    struct WindowProps
    {
        uint32_t Width = 1280, Height = 720;
        int XPos = 0, YPos = 0;

        bool Focus = true;
        bool VSync = false;
        bool HasTitleBar = true;
        bool Resizable = false;

        std::string Title = "VEXEL ENGINE";
    };

    class Window
    {
      protected:
        using EventCallbackFn = std::function<void(Scope<Event>)>;

      public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;
        virtual void PollEvents() = 0;

        virtual bool ShouldClose() const = 0;
        virtual void Close() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void SetEventCallbackFn(const EventCallbackFn& fn) = 0;

        virtual void* GetNativeWindow() const = 0;
        static uint8_t GetNumberOfWindows() { return s_NumberOfWindows; }

        static Ref<Window> Create(const WindowProps& props = WindowProps());

        static bool CreateContext();
        static void DestroyContext();

      protected:
        static inline uint8_t s_NumberOfWindows = 0;
    };
} // namespace Vex
