#include "pch.hpp"
#include "render/camera/camera_controller.hpp"
#include "input.hpp"
#include "audio/audio_engine.hpp"

void CameraController::SetPosition(const glm::vec3 &position)
{
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
    if (m_Paused)
        return;

    float newFov = m_ZoomComponent.Update(m_FieldOfView, dt);
    if (newFov != m_FieldOfView)
    {
        m_FieldOfView = newFov;
        m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
    }

    glm::vec3 newPos = m_Camera.GetPosition();

    if (Input::IsKeyPressed(Key::W))
        newPos += m_Camera.GetForward() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::S))
        newPos -= m_Camera.GetForward() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::A))
        newPos -= m_Camera.GetRight() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::D))
        newPos += m_Camera.GetRight() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::Space))
        newPos += m_Camera.GetUp() * m_CameraSpeed * dt;
    if (Input::IsKeyPressed(Key::LeftShift))
        newPos -= m_Camera.GetUp() * m_CameraSpeed * dt;

    m_Camera.SetPosition(newPos);
}

void FreeCameraController::OnResize(float width, float height)
{
    m_AspectRatio = width / height;
    m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
}

void FreeCameraController::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(FreeCameraController::OnWindowResize));
    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(FreeCameraController::OnMouseMoved));
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(FreeCameraController::OnMouseScrolled));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(FreeCameraController::OnKeyPressed));
    dispatcher.dispatch<KeyReleasedEvent>(BIND_EVENT_FN(FreeCameraController::OnKeyReleased));
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
        return true;
    }

    if (!m_Paused)
    {
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

        AudioEngine::SetListenerOrientation(m_Camera.GetForward(), m_Camera.GetUp());
    }

    m_LastX = e.getX();
    m_LastY = e.getY();
    return true;
}

bool FreeCameraController::OnMouseScrolled(MouseScrolledEvent &e)
{
    if (m_Paused)
        return false;

    float newFov = m_ZoomComponent.OnMouseScrolled(e, m_FieldOfView);
    if (newFov != m_FieldOfView)
    {
        m_FieldOfView = newFov;
        m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
        return true;
    }
    return false;
}

bool FreeCameraController::OnKeyPressed(KeyPressedEvent &e)
{
    if (m_Paused)
        return false;

    return m_ZoomComponent.OnKeyPressed(e);
}

bool FreeCameraController::OnKeyReleased(KeyReleasedEvent &e)
{
    if (m_Paused)
        return false;

    return m_ZoomComponent.OnKeyReleased(e);
}

// pitch +-90 nefunguje S/W nevim co s tim
void CreativeCameraController::OnUpdate(float dt)
{
    if (m_Paused)
        return;

    float newFov = m_ZoomComponent.Update(m_FieldOfView, dt);
    if (newFov != m_FieldOfView)
    {
        m_FieldOfView = newFov;
        m_Camera.SetProjection(m_FieldOfView, m_AspectRatio, NEAR_CLIP, FAR_CLIP);
    }

    glm::vec3 forward = m_Camera.GetForward();
    glm::vec3 forwardXZ(forward.x, 0.0f, forward.z);

    // Guard against looking straight up/down
    if (glm::dot(forwardXZ, forwardXZ) > 0.0001f)
        forwardXZ = glm::normalize(forwardXZ);

    glm::vec3 right = glm::normalize(
        glm::cross(forwardXZ, glm::vec3(0.0f, 1.0f, 0.0f)));

    glm::vec3 moveDir(0.0f);

    if (Input::IsKeyPressed(Key::LeftControl))
        m_CameraSpeed = DEFAULT_CAMERA_SPEED * 5.0f;
    else
        m_CameraSpeed = DEFAULT_CAMERA_SPEED;

    if (Input::IsKeyPressed(Key::W))
        moveDir += forwardXZ;
    if (Input::IsKeyPressed(Key::S))
        moveDir -= forwardXZ;
    if (Input::IsKeyPressed(Key::A))
        moveDir -= right;
    if (Input::IsKeyPressed(Key::D))
        moveDir += right;

    if (Input::IsKeyPressed(Key::Space))
        moveDir.y += 1.0f;
    if (Input::IsKeyPressed(Key::LeftShift))
        moveDir.y -= 1.0f;

    // Normalize to prevent diagonal speed boost
    if (glm::dot(moveDir, moveDir) > 0.0001f)
        moveDir = glm::normalize(moveDir);

    // Only update position if there's input to prevent unnecessary updates
    if (moveDir != glm::vec3(0.0f))
    {
        m_Camera.SetPosition(moveDir * m_CameraSpeed * dt + m_Camera.GetPosition());
        AudioEngine::SetListenerPosition(m_Camera.GetPosition());
    }
}