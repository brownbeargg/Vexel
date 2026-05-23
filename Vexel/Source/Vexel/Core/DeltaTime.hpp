#pragma once

namespace Vex
{
    class DeltaTime
    {
      public:
        DeltaTime(float time) : m_Time(time) {}
        void Calculate(float& last);

        float Sec() const { return m_Time; }
        float Milli() const { return m_Time * 1'000.0f; }
        float Micro() const { return m_Time * 1'000'000.0f; }

        operator float() const { return m_Time; }

      private:
        float m_Time = 0;
    };

    using TimeStep = DeltaTime;
} // namespace Vex
