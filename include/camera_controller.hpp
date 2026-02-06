#pragma once
#include "camera.hpp"
#include "event/event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"

class CameraController
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event &e) = 0;
    virtual void OnResize(float width, float height) = 0;

    void SetPosition(const glm::vec3 &position);

    virtual CameraNew &GetCamera() = 0;

protected:
    float m_AspectRatio;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
};

class PerspectiveCameraController : public CameraController
{
protected:
    static constexpr float NETURAL_FOV = 70.0f;
    static constexpr float MIN_FOV = 1.0f;

    static constexpr float NEAR_CLIP = 0.1f;
    static constexpr float FAR_CLIP = 100.0f;

public:
    PerspectiveCameraController(float aspectRatio, float fov = NETURAL_FOV, float nearClip = NEAR_CLIP, float farClip = FAR_CLIP);
    virtual ~PerspectiveCameraController() = default;

    void SetPitchYaw(float pitch, float yaw);

    PerspectiveCamera &GetCamera() override { return m_Camera; }

protected:
    PerspectiveCamera m_Camera;

    float m_FieldOfView = 0.0f;

    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
};

class FreeCameraController : public PerspectiveCameraController
{
protected:
    static constexpr float ZOOM_FOV = 15.0f;
    static constexpr float ZOOM_SNAP_EPSILON = 0.01f;

public:
    FreeCameraController(float aspectRatio, float fov = NETURAL_FOV, float nearClip = NEAR_CLIP, float farClip = FAR_CLIP)
        : PerspectiveCameraController(aspectRatio, fov, nearClip, farClip) {}
    virtual ~FreeCameraController() = default;

    void OnUpdate(float dt) override;
    void OnEvent(Event &e) override;
    void OnResize(float width, float height) override;

private:
    bool OnMouseMoved(MouseMovedEvent &e);
    bool OnMouseScrolled(MouseScrolledEvent &e);
    bool OnWindowResize(WindowResizeEvent &e);

private:
    bool m_FirstMouse = true;
    float m_LastX = 1280 / 2.0f;
    float m_LastY = 720 / 2.0f;
};