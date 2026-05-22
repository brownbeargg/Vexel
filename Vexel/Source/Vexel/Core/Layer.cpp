#include "Layer.hpp"

#include "Vexel/Core/Application.hpp"

namespace Vex
{
    Weak<Application> Layer::App()
    {
        return m_App;
    }

    void Layer::SetApp(Weak<Application> app)
    {
        m_App = app;
    }
} // namespace Vex
