#include "camera_controller.hpp"
#include "input.hpp"
#include "key_codes.hpp"
#include <iostream>

PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip)
    : m_Camera(fov, aspectRatio, nearClip, farClip)
{
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
}

void FreeCameraController::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<MouseMovedEvent>(std::bind(&FreeCameraController::OnMouseMoved, this, std::placeholders::_1));
}

bool FreeCameraController::OnMouseMoved(MouseMovedEvent &e)
{
    std::cout << "Mouse moved: " << e.getX() << ", " << e.getY() << std::endl;
    return true;
}