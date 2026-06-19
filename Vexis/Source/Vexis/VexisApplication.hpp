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

      private:
        void OnWindowClose(WindowClosedEvent& e) override;

      private:
        std::vector<Ref<Window>> m_Windows;
    };
} // namespace Vex
