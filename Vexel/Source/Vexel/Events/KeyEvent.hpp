#pragma once

#include "Vexel/Utils.hpp"

namespace Vex
{
    class KeyEvent : public Event
    {
      public:
        virtual ~KeyEvent() = default;

        VEX_EVENT_CLASS_CATEGORY(EventCategory::Input | EventCategory::Keyboard)

        uint16_t GetKeyCode() { return m_KeyCode; }

      protected:
        KeyEvent(uint16_t keyCode) : m_KeyCode(keyCode) {}

      private:
        uint16_t m_KeyCode;
    };

    class KeyPressedEvent final : public KeyEvent
    {
      public:
        KeyPressedEvent(uint16_t keyCode, int repeatCount) : KeyEvent(keyCode), m_RepeatCount(repeatCount) {}

        VEX_EVENT_CLASS_TYPE(EventType::KeyPressed)

      private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent final : public KeyEvent
    {
      public:
        KeyReleasedEvent(uint16_t keyCode) : KeyEvent(keyCode) {}

        VEX_EVENT_CLASS_TYPE(EventType::KeyReleased)
    };
} // namespace Vex
