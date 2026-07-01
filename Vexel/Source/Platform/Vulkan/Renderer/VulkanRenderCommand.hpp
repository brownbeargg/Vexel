#pragma once

#include <vulkan/vulkan_raii.hpp>

#include <glm/glm.hpp>

namespace Vex
{
    class VertexBuffer;
    class IndexBuffer;

    class VulkanRenderCommand final
    {
      public:
        static void DrawIndexed(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer);

        static void Init();
        static void Shutdown();

        static void BeginFrame(const glm::vec3& clearColor);
        static void EndFrame();

        static u32 GetImageIndex() { return s_ImageIndex; }

      private:
        static inline std::vector<vk::raii::Semaphore> s_PresentCompleteSemaphores;
        static inline std::vector<vk::raii::Semaphore> s_RenderFinishedSemaphores;
        static inline std::vector<vk::raii::Fence> s_InFlightFences;

        static inline u32 s_ImageIndex = 0;

        friend class VulkanContext;
    };
} // namespace Vex
