#pragma once
#include "key_codes.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"

class ZoomCameraComponent
{
    static constexpr float DEFAULT_ZOOM_FOV = 15.0f;
    static constexpr float DEFAULT_MIN_FOV = 1.0f;
    static constexpr float DEFAULT_ZOOM_SNAP_EPSILON = 0.01f;
    static constexpr float DEFAULT_ZOOM_EASE = 0.12f;
    static constexpr KeyCode DEFAULT_ZOOM_KEY = Key::LeftSuper;

public:
    ZoomCameraComponent(float zoomedOutFOV,
                        float zoomedInFOV = DEFAULT_ZOOM_FOV,
                        float scrollZoomMinFOV = DEFAULT_MIN_FOV,
                        float snapEpsilon = DEFAULT_ZOOM_SNAP_EPSILON,
                        float ease = DEFAULT_ZOOM_EASE,
                        KeyCode zoomKey = DEFAULT_ZOOM_KEY);

    float OnMouseScrolled(MouseScrolledEvent &e, float fov);
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnKeyReleased(KeyReleasedEvent &e);

    float Update(float fov, float dt);

    float GetZoomedInFOV() const { return m_ZoomedInFOV; }
    void SetZoomedInFOV(float fov) { m_ZoomedInFOV = fov; }

    float GetZoomedOutFOV() const { return m_ZoomedOutFOV; }
    void SetZoomedOutFOV(float fov) { m_ZoomedOutFOV = fov; }

    float GetScrollZoomMinFOV() const { return m_ScrollZoomMinFOV; }
    void SetScrollZoomMinFOV(float fov) { m_ScrollZoomMinFOV = fov; }

    float GetEase() const { return m_Ease; }
    void SetEase(float ease) { m_Ease = ease; }

private:
    float m_ZoomedInFOV, m_ZoomedOutFOV, m_ScrollZoomMinFOV;
    float m_SnapEpsilon;
    float m_Ease;
    KeyCode m_ZoomKey;

    enum class ZoomState
    {
        ZOOMED_OUT,
        ZOOMING_IN,
        ZOOMED_IN,
        ZOOMING_OUT
    };
    ZoomState m_ZoomState = ZoomState::ZOOMED_OUT;
};