#pragma once

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(const glm::mat4 &projection) : m_ProjectionMatrix(projection) {}
    virtual ~Camera() = default;

    void SetPosition(const glm::vec3 &position);
    const glm::vec3 &GetPosition() const { return m_Position; }

    const glm::mat4 &GetProjectionMatrix() const { return m_ProjectionMatrix; }
    const glm::mat4 &GetViewMatrix() const { return m_ViewMatrix; }
    const glm::mat4 &GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

private:
    virtual void RecalculateViewMatrix() = 0;

protected:
    glm::mat4 m_ProjectionMatrix;
    glm::mat4 m_ViewMatrix;
    // cached value of Projection * View, as it's used often
    glm::mat4 m_ViewProjectionMatrix;

    glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
};

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

    void SetProjection(float fov, float aspectRatio, float nearClip, float farClip);

    void SetPitchYaw(float pitch, float yaw);
    float GetPitch() const { return m_Pitch; }
    float GetYaw() const { return m_Yaw; }

    const glm::vec3 &GetForward() const { return m_Forward; };
    const glm::vec3 &GetRight() const { return m_Right; };
    const glm::vec3 &GetUp() const { return m_Up; };

private:
    void RecalculateViewMatrix() override;

private:
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;

    glm::vec3 m_Forward;
    glm::vec3 m_Up;
    glm::vec3 m_Right;
};

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float left, float right, float bottom, float top);

    void SetProjection(float left, float right, float bottom, float top);

    void SetRotation(float rotation);
    float GetRotation() const { return m_Rotation; }

private:
    void RecalculateViewMatrix() override;

private:
    float m_Rotation = 0.0f;
};