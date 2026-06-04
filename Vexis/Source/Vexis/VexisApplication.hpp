#pragma once

#include <Vexel/Vexel.hpp>

namespace Vex
{
    class VexisApplication final : public Application
    {
      public:
        VexisApplication();
        ~VexisApplication();

        void OnUpdate() override;
        void PollEvents() override;

      protected:
        void OnWindowClose(WindowClosedEvent& e) override
        {
            /// @todo add == operator for Refs in VexelUtils
            if (m_Window->ShouldClose() && e.GetWindow() == m_Window)
            {
                m_Window->Close();
                if (Window::GetNumberOfWindows() == 0)
                    Close();
            }
        }
    };
} // namespace Vex
