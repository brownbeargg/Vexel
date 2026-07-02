#pragma once

#include "Vexel/Core/Window.hpp"

#include <glm/glm.hpp>

namespace Vex
{
    class VertexBuffer;
    class IndexBuffer;

    enum class GraphicsAPIs
    {
        Vulkan
    };

    class RendererAPI
    {
      public:
        static void DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer);

        static void Init();
        static void Shutdown();

        static GraphicsAPIs API() { return s_GraphicsAPI; }

        static void BeginFrame(const glm::vec3& clearColor);
        static void EndFrame();

      private:
        inline static GraphicsAPIs s_GraphicsAPI = GraphicsAPIs::Vulkan;
    };
} // namespace Vex
