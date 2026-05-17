#pragma once

#include "Vexel/Core/LayerStack.hpp"
#include "Vexel/Memory.hpp"

namespace Vex
{
    class Application : public RefCount
    {
      public:
        virtual ~Application() = default;

        void Run();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void PopLayer(Layer* layer);
        void PopOverlay(Layer* overlay);

        /**
         * @brief this function will be defined in the client
         */
        static Application* Create();

      protected:
        Application() = default;

      private:
        bool m_Running = true;

        LayerStack m_LayerStack = {Weak<Application>(this)};
    };
} // namespace Vex
