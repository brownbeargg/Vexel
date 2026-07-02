#pragma once

namespace Vex
{
    class Window;
    class VulkanContext;

    class RendererContext
    {
      public:
        virtual ~RendererContext() = default;

        static Ref<RendererContext> Create(Observer<Window> pWindow);

        virtual void Init() = 0;

        virtual void MakeContextCurrent() = 0;
        virtual void WaitForGPUIdle() = 0;

        VulkanContext* ToVulkanContext();
    };
} // namespace Vex
