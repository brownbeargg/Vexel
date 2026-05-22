#pragma once

#include "Vexel/Core/LayerStack.hpp"
#include "Vexel/Events/ApplicationEvent.hpp"

#include "Vexel/Utils.hpp"

namespace Vex
{
    class Application : public RefCount
    {
      public:
        virtual ~Application();

        void Run();

        virtual void PollEvents() = 0;
        void ForwardEvent(Scope<Event> e);

        virtual void OnUpdate() = 0;

        void Close() { m_Running = false; }

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        /**
         * @brief this function will be defined in the client
         */
        static Application* Create();

      protected:
        Application();

        virtual void OnWindowClose(WindowClosedEvent& e) = 0;

      protected:
        Vex::EventBus m_EventBus;

      private:
        bool m_Running = true;

        LayerStack m_LayerStack = Weak<Application>(this);
    };
} // namespace Vex
