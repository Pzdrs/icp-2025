#include "camera.hpp"

/////////////////////////
// PERSPECTIVE CAMERA
/////////////////////////

PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
    : m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
{
    RecalculateViewMatrix();
}

void PerspectiveCamera::SetProjection(float fov, float aspectRatio, float nearClip, float farClip)
{
    m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
    RecalculateViewMatrix();
}

void PerspectiveCamera::SetPitchYaw(float pitch, float yaw)
{
    // lock the pitch in <-89, 89> degrees
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    m_Pitch = pitch;
    m_Yaw = yaw;
    RecalculateViewMatrix();
}

void PerspectiveCamera::SetPosition(const glm::vec3 &position)
{
    m_Position = position;
    RecalculateViewMatrix();
}

void PerspectiveCamera::RecalculateViewMatrix()
{
    // Convert pitch/yaw to forward vector
    glm::vec3 forward;
    forward.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    forward.y = sin(glm::radians(m_Pitch));
    forward.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
    m_Forward = glm::normalize(forward);

    glm::vec3 worldUp = {0.0f, 1.0f, 0.0f};
    m_Right = glm::normalize(glm::cross(m_Forward, worldUp));
    m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

    m_ViewMatrix = glm::lookAt(
        m_Position,             // Camera position
        m_Position + m_Forward, // Look at point
        m_Up                    // Up vector
    );
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

///////////////////////
// ORTHOGRAPHIC CAMERA
///////////////////////

OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
    : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
{
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
{
    m_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void OrthographicCamera::RecalculateViewMatrix()
{
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *
                          glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

    m_ViewMatrix = glm::inverse(transform);
    m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}