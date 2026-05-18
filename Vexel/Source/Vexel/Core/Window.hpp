#pragma once

#include "Vexel/Memory.hpp"

namespace Vex
{
    struct WindowProps
    {
        std::string Title = "VEXEL ENGINE";
        uint32_t Width = 1280, Height = 720;
        int XPos{}, YPos{};
        bool Focus = true;
        bool HasTitleBar = true;
        bool VSync = false;
    };

    class Window : public RefCount
    {
      public:
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Ref<Window> Create(const WindowProps& props = WindowProps());

        static bool CreateContext();
        static void DestroyContext();
    };
} // namespace Vex
