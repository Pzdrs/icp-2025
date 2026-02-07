#include "pch.hpp"
#include "render/camera/camera_component.hpp"
#include <glm/glm.hpp>

ZoomCameraComponent::ZoomCameraComponent(float zoomedOutFOV, float zoomedInFOV, float scrollZoomMinFOV, float snapEpsilon, float ease, KeyCode zoomKey)
    : m_ZoomedInFOV(zoomedInFOV),
      m_ZoomedOutFOV(zoomedOutFOV),
      m_ScrollZoomMinFOV(scrollZoomMinFOV),
      m_SnapEpsilon(snapEpsilon),
      m_Ease(ease),
      m_ZoomKey(zoomKey)
{
}

bool ZoomCameraComponent::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == m_ZoomKey)
    {
        m_ZoomState = ZoomState::ZOOMING_IN;
        return true;
    }
    return false;
}

bool ZoomCameraComponent::OnKeyReleased(KeyReleasedEvent &e)
{
    if (e.getKeyCode() == m_ZoomKey)
    {
        m_ZoomState = ZoomState::ZOOMING_OUT;
        return true;
    }
    return false;
}

float ZoomCameraComponent::OnMouseScrolled(MouseScrolledEvent &e, float fov)
{
    if (m_ZoomState != ZoomState::ZOOMED_IN)
        return fov;

    return glm::clamp(fov - e.getYOffset(), m_ScrollZoomMinFOV, m_ZoomedOutFOV);
}

float ZoomCameraComponent::Update(float fov, float dt)
{
    if (m_ZoomState == ZoomState::ZOOMED_IN || m_ZoomState == ZoomState::ZOOMED_OUT)
        return fov;

    if (m_ZoomState == ZoomState::ZOOMING_IN)
    {
        fov += (m_ZoomedInFOV - fov) * m_Ease;
        if (fabs(fov - m_ZoomedInFOV) < m_SnapEpsilon)
        {
            m_ZoomState = ZoomState::ZOOMED_IN;
            return m_ZoomedInFOV;
        }
    }
    else if (m_ZoomState == ZoomState::ZOOMING_OUT)
    {
        fov += (m_ZoomedOutFOV - fov) * m_Ease;

        if (fabs(fov - m_ZoomedOutFOV) < m_SnapEpsilon)
        {
            m_ZoomState = ZoomState::ZOOMED_OUT;
            return m_ZoomedOutFOV;
        }
    }
    return fov;
}