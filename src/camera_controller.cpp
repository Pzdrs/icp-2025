#include "camera_controller.hpp"
#include "input.hpp"
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
    tickZoom(dt);

    if (Input::IsKeyPressed(Key::W))
        m_CameraPosition += m_Camera.GetForward() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::S))
        m_CameraPosition -= m_Camera.GetForward() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::A))
        m_CameraPosition -= m_Camera.GetRight() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::D))
        m_CameraPosition += m_Camera.GetRight() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::Space))
        m_CameraPosition += m_Camera.GetUp() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::LeftShift))
        m_CameraPosition -= m_Camera.GetUp() * m_CameraSpeed * dt;

    m_Camera.SetPosition(m_CameraPosition);
}

void FreeCameraController::OnResize(float width, float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
}

void FreeCameraController::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(std::bind(&FreeCameraController::OnWindowResize, this, std::placeholders::_1));
    dispatcher.dispatch<MouseMovedEvent>(std::bind(&FreeCameraController::OnMouseMoved, this, std::placeholders::_1));
    dispatcher.dispatch<MouseScrolledEvent>(std::bind(&FreeCameraController::OnMouseScrolled, this, std::placeholders::_1));
    dispatcher.dispatch<KeyPressedEvent>(std::bind(&FreeCameraController::OnKeyPressed, this, std::placeholders::_1));
    dispatcher.dispatch<KeyReleasedEvent>(std::bind(&FreeCameraController::OnKeyReleased, this, std::placeholders::_1));
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
    float yoffset = e.getY() - m_LastY; // raw delta, moving mouse up = negative Y

    if (!m_InvertMouse)
        yoffset = -yoffset;

    xoffset *= m_MouseSensitivity;
    yoffset *= m_MouseSensitivity;

    m_Yaw += xoffset;
    m_Pitch = glm::clamp(m_Pitch + yoffset, -90.0f, 90.0f);

    if (m_Yaw > 180.0f)
        m_Yaw -= 360.0f;
    if (m_Yaw < -180.0f)
        m_Yaw += 360.0f;

    m_Camera.SetPitchYaw(m_Pitch, m_Yaw);

    m_LastX = e.getX();
    m_LastY = e.getY();
    return true;
}

bool FreeCameraController::OnMouseScrolled(MouseScrolledEvent &e)
{
    if (m_ZoomState != ZoomState::ZOOMED_IN)
        return true;

    m_FieldOfView -= e.getYOffset();
    m_FieldOfView = glm::clamp(m_FieldOfView, MIN_FOV, NETURAL_FOV);
    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
    return true;
}

bool FreeCameraController::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == ZOOM_KEY)
    {
        m_ZoomState = ZoomState::ZOOMING_IN;
        return true;
    }
    return false;
}

bool FreeCameraController::OnKeyReleased(KeyReleasedEvent &e)
{
    if (e.getKeyCode() == ZOOM_KEY)
    {
        m_ZoomState = ZoomState::ZOOMING_OUT;
        return true;
    }
    return false;
}

void FreeCameraController::tickZoom(float dt)
{
    if (m_ZoomState == ZoomState::ZOOMED_IN || m_ZoomState == ZoomState::ZOOMED_OUT)
        return;

    if (m_ZoomState == ZoomState::ZOOMING_IN)
    {
        m_FieldOfView += (ZOOM_FOV - m_FieldOfView) * ZOOM_EASE;

        if (fabs(m_FieldOfView - ZOOM_FOV) < ZOOM_SNAP_EPSILON)
        {
            m_FieldOfView = ZOOM_FOV;
            m_ZoomState = ZoomState::ZOOMED_IN;
        }
    }
    else if (m_ZoomState == ZoomState::ZOOMING_OUT)
    {
        m_FieldOfView += (NETURAL_FOV - m_FieldOfView) * ZOOM_EASE;

        if (fabs(m_FieldOfView - NETURAL_FOV) < ZOOM_SNAP_EPSILON)
        {
            m_FieldOfView = NETURAL_FOV;
            m_ZoomState = ZoomState::ZOOMED_OUT;
        }
    }

    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
}