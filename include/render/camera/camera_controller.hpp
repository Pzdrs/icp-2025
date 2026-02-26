#pragma once
#include "render/camera/camera.hpp"
#include "event/event.hpp"
#include "event/mouse_event.hpp"
#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "key_codes.hpp"
#include "render/camera/camera_component.hpp"

class CameraController
{
public:
    virtual void OnUpdate(float dt) = 0;
    virtual void OnEvent(Event &e) = 0;
    virtual void OnResize(float width, float height) = 0;

    void SetPosition(const glm::vec3 &position);

    void SetPaused(bool paused) { m_Paused = paused; }
    bool IsPaused() const { return m_Paused; }

    virtual Camera &GetCamera() = 0;

protected:
    float m_AspectRatio;
    bool m_Paused = false;
};

class PerspectiveCameraController : public CameraController
{
protected:
    static constexpr float DEFAULT_FOV = 70.0f;

    static constexpr float NEAR_CLIP = 0.1f;
    static constexpr float FAR_CLIP = 500.0f;

public:
    PerspectiveCameraController(float aspectRatio, float fov = DEFAULT_FOV, float nearClip = NEAR_CLIP, float farClip = FAR_CLIP);
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
    static constexpr float DEFAULT_CAMERA_SPEED = 10.0f;
    static constexpr float DEFAULT_MOUSE_SENSITIVITY = 0.1f;

public:
    FreeCameraController(float aspectRatio, float fov = DEFAULT_FOV, float nearClip = NEAR_CLIP, float farClip = FAR_CLIP)
        : PerspectiveCameraController(aspectRatio, fov, nearClip, farClip),
          m_CameraSpeed(DEFAULT_CAMERA_SPEED),
          m_MouseSensitivity(DEFAULT_MOUSE_SENSITIVITY),
          m_ZoomComponent(fov) {}
    virtual ~FreeCameraController() = default;

    void OnUpdate(float dt) override;
    void OnEvent(Event &e) override;
    void OnResize(float width, float height) override;

    void SetCameraSpeed(float speed) { m_CameraSpeed = speed; }
    void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }
    void SetInvertMouse(bool invert) { m_InvertMouse = invert; }
    void ResetMouse() { m_FirstMouse = true; }

    ZoomCameraComponent &GetZoomComponent() { return m_ZoomComponent; }

private:
    bool OnMouseMoved(MouseMovedEvent &e);
    bool OnMouseScrolled(MouseScrolledEvent &e);
    bool OnWindowResize(WindowResizeEvent &e);
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnKeyReleased(KeyReleasedEvent &e);

protected:
    ZoomCameraComponent m_ZoomComponent;

    bool m_FirstMouse = true;
    float m_LastX = 0.0;
    float m_LastY = 0.0;

    float m_CameraSpeed;
    float m_MouseSensitivity;
    bool m_InvertMouse = false;
};

class CreativeCameraController : public FreeCameraController
{
public:
    using FreeCameraController::FreeCameraController;

    void OnUpdate(float dt) override;
};