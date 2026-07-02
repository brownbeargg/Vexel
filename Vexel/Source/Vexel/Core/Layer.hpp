#pragma once

#include "Vexel/Core/DeltaTime.hpp"
#include "Vexel/Utils.hpp"

namespace Vex
{
    class Application;

    class Layer
    {
      public:
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}

        virtual void OnUpdate(DeltaTime dt) {}
        virtual void OnFixedUpdate(TimeStep ts) {}

        virtual void OnEvent(Event& e) {}

        virtual void OnImGuiRender() {}

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
