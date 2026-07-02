#pragma once

#include <Vexel/Vexel.hpp>

namespace Vex
{
    class VexisApplication final : public Application
    {
      public:
        VexisApplication();
        ~VexisApplication();

      private:
        void OnWindowClose(WindowClosedEvent& e) override;
    };
} // namespace Vex
