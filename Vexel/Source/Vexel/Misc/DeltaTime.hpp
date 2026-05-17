#pragma once

namespace Vex
{
    class DeltaTime
    {
      public:
        DeltaTime(float time) : m_Time(time) {}
        void Calculate();

        float Sec() { return m_Time; }
        float Milli() { return m_Time * 1000.0f; }
        float Micro() { return m_Time * 1000000.0f; }

        operator float() { return m_Time; }

      private:
        float m_Time = 0;
    };

    using TimeStep = DeltaTime;
} // namespace Vex
