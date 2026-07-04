#include "EditorLayer.hpp"

namespace Vex
{
    EditorLayer::EditorLayer() : Layer("EditorLayer")
    {
        m_QuadUniformBuffer = UniformBuffer::Create();
        m_TriangleUniformBuffer = UniformBuffer::Create();

        m_Shader = Shader::Create(
            RootDirectory::Assets, "Shaders/Basic.vert.spv", "Shaders/Basic.frag.spv", m_QuadUniformBuffer);
        std::vector<VertexData> quadVertices = {
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Bottom left
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom Right
            {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top right
            {{-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}} // Top left
        };

        std::vector<u32> quadIndices = {0, 1, 2, 0, 2, 3};

        m_QuadVertexBuffer = VertexBuffer::Create(quadVertices);
        m_QuadIndexBuffer = IndexBuffer::Create(quadIndices);

        std::vector<VertexData> triangleVertices = {
            {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}}, // Bottom left
            {{0.5f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},  // Bottom Right
            {{0.0f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}}, // Top
        };

        std::vector<u32> triangleIndices = {0, 1, 2};

        m_TriangleVertexBuffer = VertexBuffer::Create(triangleVertices);
        m_TriangleIndexBuffer = IndexBuffer::Create(triangleIndices);

        m_QuadMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.5f, 0.5f, 0.0f));
        m_TriangleMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));

        Ref<Window> window = Application::GetActiveWindows()[0];
        m_Camera = Ref<PerspectiveCamera>::Create(
            45.0f, (float)window->GetWidth() / (float)window->GetHeight(), 0.1f, 400.0f);
        m_Camera->SetPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    }

    void EditorLayer::OnUpdate(DeltaTime dt)
    {
        m_FPS = 1 / dt;

        HandleInput();

        m_QuadMVP.Model =
            glm::rotate(m_QuadMVP.Model, glm::radians(10 * dt.Sec()), glm::vec3{0.0f, 0.0f, 1.0f});
        m_QuadMVP.View = m_Camera->GetViewMatrix();
        m_QuadMVP.Projection = m_Camera->GetProjectionMatrix();

        m_QuadUniformBuffer->Invalidate(m_QuadMVP);
        m_QuadUniformBuffer->Bind(m_Shader);

        m_Shader->Bind();
        RendererAPI::DrawIndexed(m_QuadVertexBuffer, m_QuadIndexBuffer);

        if (m_TriangleBacking)
        {
            m_TriangleDistance += m_TriangleSpeed * dt;

            if (m_TriangleDistance >= m_MaxTriangleDistance)
                m_TriangleBacking = false;

            if (m_TriangleDistance > m_MaxTriangleDistance)
                m_TriangleDistance = m_MaxTriangleDistance;
        }
        else
        {
            m_TriangleDistance -= m_TriangleSpeed * dt;

            if (m_TriangleDistance <= m_MinTriangleDistance)
                m_TriangleBacking = true;

            if (m_TriangleDistance < m_MinTriangleDistance)
                m_TriangleDistance = m_MinTriangleDistance;
        }

        m_TriangleMVP.Model = glm::translate(m_TriangleMVP.Model, glm::vec3(0.0f, 0.0f, m_TriangleDistance));

        m_TriangleUniformBuffer->Invalidate(m_TriangleMVP);
        m_TriangleUniformBuffer->Bind(m_Shader);

        m_TriangleMVP.Model = glm::translate(glm::mat4(1.0f), glm::vec3(-0.5f, -0.5f, 0.0f));
        m_TriangleMVP.View = m_Camera->GetViewMatrix();
        m_TriangleMVP.Projection = m_Camera->GetProjectionMatrix();

        m_Shader->Bind();
        RendererAPI::DrawIndexed(m_TriangleVertexBuffer, m_TriangleIndexBuffer);
    }

    void EditorLayer::OnImGuiRender()
    {
        ImGui::Begin("Engine statistics");
        ImGui::Value("FPS:", m_FPS);
        ImGui::End();

        ImGui::Begin("Settings");
        ImGui::SliderFloat("Max Triangle Distance", &m_MaxTriangleDistance, 0.0f, 40.0f);
        ImGui::SliderFloat("Min Triangle Distance", &m_MinTriangleDistance, 0.0f, 40.0f);
        ImGui::Value("Current triangle distance", m_TriangleDistance);
        ImGui::End();
    }

    void EditorLayer::HandleInput()
    {
        if (Input::IsKeyPressed(Key::W))
            m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraSpeed * m_Camera->GetFront());
        if (Input::IsKeyPressed(Key::S))
            m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraSpeed * m_Camera->GetFront());
        if (Input::IsKeyPressed(Key::A))
            m_Camera->SetPosition(m_Camera->GetPosition() -
                m_CameraSpeed * glm::cross(m_Camera->GetFront(), m_Camera->GetUp()));
        if (Input::IsKeyPressed(Key::D))
            m_Camera->SetPosition(m_Camera->GetPosition() +
                m_CameraSpeed * glm::cross(m_Camera->GetFront(), m_Camera->GetUp()));
        if (Input::IsKeyPressed(Key::Q))
            m_Camera->SetPosition(m_Camera->GetPosition() + (0.2f * m_CameraSpeed) * m_Camera->GetUp());
        if (Input::IsKeyPressed(Key::SPACE))
            m_Camera->SetPosition(m_Camera->GetPosition() - (0.2f * m_CameraSpeed) * m_Camera->GetUp());

        if (Input::IsMouseButtonPressed(Mouse::RIGHT))
        {
            const_cast<Ref<Window>&>(Application::GetActiveWindows()[0])->ListenToMouse(true);

            glm::vec2 mousePos = Input::GetMousePos();
            glm::vec2 offset{mousePos.x - m_LastMousePos.x, mousePos.y - m_LastMousePos.y};

            constexpr float sensitivity = 0.15f;
            offset *= sensitivity;
            m_CameraRotation += glm::vec3{offset, 0.0f};

            if (m_CameraRotation.y > 89.0f)
                m_CameraRotation.y = 89.0f;
            if (m_CameraRotation.y < -89.0f)
                m_CameraRotation.y = -89.0f;

            glm::vec3 direction;
            direction.x =
                glm::cos(glm::radians(m_CameraRotation.x)) * glm::cos(glm::radians(m_CameraRotation.y));
            direction.y = glm::sin(glm::radians(m_CameraRotation.y));
            direction.z =
                glm::sin(glm::radians(m_CameraRotation.x)) * glm::cos(glm::radians(m_CameraRotation.y));

            m_Camera->SetFront(direction);
            glm::vec3 right = glm::normalize(glm::cross(m_Camera->GetFront(), {0.0f, 1.0f, 0.0f}));
            m_Camera->SetUp(glm::normalize(glm::cross(right, m_Camera->GetFront())));
        }
        else
        {
            const_cast<Ref<Window>&>(Application::GetActiveWindows()[0])->ListenToMouse(false);
        }

        m_LastMousePos = Input::GetMousePos();
    }
} // namespace Vex
