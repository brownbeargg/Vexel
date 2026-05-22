#pragma once

#include "Vexel/Utils.hpp"

namespace Vex
{
    /// @todo add own mousecodes
    class MouseButtonEvent : public Event
    {
      public:
        virtual ~MouseButtonEvent() = default;

        VEX_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

        int GetMouseButton() { return m_Button; }

      protected:
        MouseButtonEvent(int button) : m_Button(button) {}

      private:
        int m_Button;
    };

    class MouseButtonPressedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        VEX_EVENT_CLASS_TYPE(EventType::MouseButtonPressed);
    };

    class MouseButtonReleasedEvent final : public MouseButtonEvent
    {
      public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        VEX_EVENT_CLASS_TYPE(EventType::MouseButtonReleased);
    };

    class MouseMovedEvent final : public Event
    {
      public:
        MouseMovedEvent(float xPos, float yPos) : m_XPos(xPos), m_YPos(yPos) {}

        VEX_EVENT_CLASS_TYPE(EventType::MouseScrolled)
        VEX_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse);

        float GetXPos() const { return m_XPos; }
        float GetYPos() const { return m_YPos; }

      private:
        float m_XPos, m_YPos;
    };

    class MouseScrolledEvent final : public Event
    {
      public:
        MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}

        VEX_EVENT_CLASS_TYPE(EventType::MouseScrolled)
        VEX_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Mouse)

        float GetXOffset() { return m_XOffset; }
        float GetYOffset() { return m_YOffset; }

      private:
        float m_XOffset, m_YOffset;
    };
} // namespace Vex
