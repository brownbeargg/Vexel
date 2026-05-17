#pragma once

#include "Vexel/Memory.hpp"
#include "Vexel/Misc/DeltaTime.hpp"

namespace Vex
{
    class Application;

    class Layer
    {
      public:
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        /// @todo add a DeltaTime class and a TimeStep alias

        virtual void OnUpdate(DeltaTime dt) {}
        virtual void OnFixedUpdate(TimeStep ts) {}

        Weak<Application> App();
        const std::string& Name() { return m_Name; }

      protected:
        Layer(const std::string& name) : m_Name(name) {}

      private:
        void SetApp(Weak<Application> app);

      private:
        Weak<Application> m_App = {};
        std::string m_Name;

        friend class LayerStack;
    };
} // namespace Vex
