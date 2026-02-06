#pragma once
#include "camera.hpp"
#include "event/event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "key_codes.hpp"

class CameraController
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event &e) = 0;
    virtual void OnResize(float width, float height) = 0;

    void SetPosition(const glm::vec3 &position);

    virtual Camera &GetCamera() = 0;

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

    float m_FieldOfView;

    float m_Pitch;
    float m_Yaw;
};

class FreeCameraController : public PerspectiveCameraController
{
protected:
    static constexpr float DEFAULT_CAMERA_SPEED = 5.0f;
    static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;

    static constexpr float ZOOM_FOV = 15.0f;
    static constexpr float ZOOM_SNAP_EPSILON = 0.01f;
    static constexpr float ZOOM_EASE = 0.12f;
    static constexpr KeyCode ZOOM_KEY = Key::LeftSuper;

public:
    FreeCameraController(float aspectRatio, float fov = NETURAL_FOV, float nearClip = NEAR_CLIP, float farClip = FAR_CLIP)
        : PerspectiveCameraController(aspectRatio, fov, nearClip, farClip),
          m_CameraSpeed(DEFAULT_CAMERA_SPEED),
          m_MouseSensitivity(DEFAULT_MOUSE_SENSITIVITY) {}
    virtual ~FreeCameraController() = default;

    void OnUpdate(float dt) override;
    void OnEvent(Event &e) override;
    void OnResize(float width, float height) override;

    void SetCameraSpeed(float speed) { m_CameraSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }
    void SetInvertMouse(bool invert) { m_InvertMouse = invert; }

private:
    bool OnMouseMoved(MouseMovedEvent &e);
    bool OnMouseScrolled(MouseScrolledEvent &e);
    bool OnWindowResize(WindowResizeEvent &e);
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnKeyReleased(KeyReleasedEvent &e);

    void tickZoom(float dt);

private:
    bool m_FirstMouse = true;
    float m_LastX = 0.0; 
    float m_LastY = 0.0;

    float m_CameraSpeed;
    float m_MouseSensitivity;
    bool m_InvertMouse = false;

    enum class ZoomState
    {
        ZOOMED_OUT,
        ZOOMING_IN,
        ZOOMED_IN,
        ZOOMING_OUT
    };
    ZoomState m_ZoomState = ZoomState::ZOOMED_OUT;
};