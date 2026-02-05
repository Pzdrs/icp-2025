#pragma once
#include "camera.hpp"
#include "event/event.hpp"
#include "event/mouse_event.hpp"

class CameraController
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event &e) = 0;
    virtual void OnResize(float width, float height) = 0;

    // set position
    void SetPosition(const glm::vec3 &position)
    {
        m_CameraPosition = position;
        GetCamera().SetPosition(position);
    }

    virtual CameraNew &GetCamera() = 0;

protected:
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
};

class PerspectiveCameraController : public CameraController
{
public:
    PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip);
    virtual ~PerspectiveCameraController() = default;

    void SetPitchYaw(float pitch, float yaw)
    {
        m_Pitch = pitch;
        m_Yaw = yaw;

        m_Camera.SetPitchYaw(pitch, yaw);
    }

    PerspectiveCamera &GetCamera() override { return m_Camera; }

protected:
    PerspectiveCamera m_Camera;

    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
};

class FreeCameraController : public PerspectiveCameraController
{
public:
    FreeCameraController(float fov, float aspectRatio, float nearClip, float farClip)
        : PerspectiveCameraController(fov, aspectRatio, nearClip, farClip) {}
    virtual ~FreeCameraController() = default;

    void OnUpdate(float dt) override;
    void OnEvent(Event &e) override;
    void OnResize(float width, float height) override;

private:
    bool OnMouseMoved(MouseMovedEvent &e);
};