#include "camera_controller.hpp"
#include "input.hpp"
#include "key_codes.hpp"
#include <iostream>

void CameraController::SetPosition(const glm::vec3 &position)
{
    m_CameraPosition = position;
    GetCamera().SetPosition(position);
}

PerspectiveCameraController::PerspectiveCameraController(float aspectRatio, float fov, float nearClip, float farClip)
    : m_Camera(fov, aspectRatio, nearClip, farClip)
{
    m_FieldOfView = fov;
    m_AspectRatio = aspectRatio;
}

void PerspectiveCameraController::SetPitchYaw(float pitch, float yaw)
{
    m_Pitch = pitch;
    m_Yaw = yaw;

    m_Camera.SetPitchYaw(pitch, yaw);
}

////////////////////////
// FreeCameraController
////////////////////////

void FreeCameraController::OnUpdate(float dt)
{
    float velocity = 5.0f * dt;

    if (Input::IsKeyPressed(Key::W))
        m_CameraPosition += m_Camera.GetForward() * velocity;
    if (Input::IsKeyPressed(Key::S))
        m_CameraPosition -= m_Camera.GetForward() * velocity;
    if (Input::IsKeyPressed(Key::A))
        m_CameraPosition -= m_Camera.GetRight() * velocity;
    if (Input::IsKeyPressed(Key::D))
        m_CameraPosition += m_Camera.GetRight() * velocity;
    if (Input::IsKeyPressed(Key::Space))
        m_CameraPosition += m_Camera.GetUp() * velocity;
    if (Input::IsKeyPressed(Key::LeftShift))
        m_CameraPosition -= m_Camera.GetUp() * velocity;

    m_Camera.SetPosition(m_CameraPosition);
}

void FreeCameraController::OnResize(float width, float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, 0.1f, 100.0f);
}

void FreeCameraController::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(std::bind(&FreeCameraController::OnWindowResize, this, std::placeholders::_1));
    dispatcher.dispatch<MouseMovedEvent>(std::bind(&FreeCameraController::OnMouseMoved, this, std::placeholders::_1));
    dispatcher.dispatch<MouseScrolledEvent>(std::bind(&FreeCameraController::OnMouseScrolled, this, std::placeholders::_1));
}

bool FreeCameraController::OnWindowResize(WindowResizeEvent &e)
{
    OnResize((float)e.getWidth(), (float)e.getHeight());
    return true;
}

bool FreeCameraController::OnMouseMoved(MouseMovedEvent &e)
{
    if (m_FirstMouse)
    {
        m_LastX = e.getX();
        m_LastY = e.getY();
        m_FirstMouse = false;
    }

    float xoffset = e.getX() - m_LastX;
    float yoffset = m_LastY - e.getY(); // reversed since y-coordinates go from bottom to top

    m_LastX = e.getX();
    m_LastY = e.getY();
    xoffset *= 0.1f;
    yoffset *= 0.1f;
    m_Yaw += xoffset;
    m_Pitch += yoffset;
    m_Camera.SetPitchYaw(m_Pitch, m_Yaw);
    return true;
}

bool FreeCameraController::OnMouseScrolled(MouseScrolledEvent &e)
{
    m_FieldOfView -= e.getYOffset();
    m_FieldOfView = glm::clamp(m_FieldOfView, MIN_FOV, NETURAL_FOV);
    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, 0.1f, 100.0f);
    return true;
}