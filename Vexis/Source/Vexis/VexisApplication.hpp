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

      private:
        Ref<Window> m_Window;
    };
} // namespace Vex
