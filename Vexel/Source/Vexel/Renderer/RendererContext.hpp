#pragma once

namespace Vex
{
    class Window;

    class RendererContext
    {
      public:
        virtual ~RendererContext() = default;

        static Ref<RendererContext> Create(Observer<Window> pWindow);

        virtual void Init() = 0;
    };
} // namespace Vex
