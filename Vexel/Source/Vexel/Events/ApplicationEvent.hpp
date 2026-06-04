#pragma once

#include "Vexel/Core/Window.hpp"
#include "Vexel/Utils.hpp"

namespace Vex
{
    class AppTickedEvent final : public Event
    {
      public:
        VEX_EVENT_CLASS_TYPE(EventType::AppTicked)
        VEX_EVENT_CLASS_CATEGORY(EventCategory::App)
    };

    class AppUpdatedEvent final : public Event
    {
      public:
        VEX_EVENT_CLASS_TYPE(EventType::AppTicked)
        VEX_EVENT_CLASS_CATEGORY(EventCategory::App)
    };

    class AppRenderedEvent final : public Event
    {
      public:
        VEX_EVENT_CLASS_TYPE(EventType::AppRendered)
        VEX_EVENT_CLASS_CATEGORY(EventCategory::App)
    };

    class WindowEvent : public Event
    {
      public:
        virtual ~WindowEvent() = default;

        VEX_EVENT_CLASS_CATEGORY(EventCategory::App)

        Observer<Window> GetWindow() { return m_Window; }

      protected:
        WindowEvent(Observer<Window> window) : m_Window(window) {}

      private:
        Observer<Window> m_Window;
    };

    class WindowClosedEvent final : public WindowEvent
    {
      public:
        WindowClosedEvent(Observer<Window> window) : WindowEvent(window) {}

        VEX_EVENT_CLASS_TYPE(EventType::WindowClosed)
    };

    class WindowResizedEvent final : public WindowEvent
    {
      public:
        WindowResizedEvent(Observer<Window> window, uint32_t width, uint32_t height)
            : WindowEvent(window), m_Width(width), m_Height(height)
        {
        }

        VEX_EVENT_CLASS_TYPE(EventType::WindowResized)

        uint32_t GetWidth() { return m_Width; }
        uint32_t GetHeight() { return m_Height; }

      private:
        uint32_t m_Width = 0, m_Height = 0;
    };

    class WindowFocusedEvent final : public WindowEvent
    {
      public:
        WindowFocusedEvent(Observer<Window> window) : WindowEvent(window) {}

        VEX_EVENT_CLASS_TYPE(EventType::WindowFocused)
    };

    class WindowLostFocusEvent final : public WindowEvent
    {
      public:
        WindowLostFocusEvent(Observer<Window> window) : WindowEvent(window) {}

        VEX_EVENT_CLASS_TYPE(EventType::WindowLostFocus)
    };

    class WindowMovedEvent final : public WindowEvent
    {
      public:
        WindowMovedEvent(Observer<Window> window, int xPos, int yPos)
            : WindowEvent(window), m_XPos(xPos), m_YPos(yPos)
        {
        }

        VEX_EVENT_CLASS_TYPE(EventType::WindowMoved)

      private:
        int m_XPos, m_YPos;
    };
} // namespace Vex
