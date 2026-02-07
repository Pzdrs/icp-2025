#pragma once

#include "layer/layer.hpp"
#include "render/camera/camera.hpp"

class DebugLayer : public Layer
{
public:
    DebugLayer(const Camera &camera);
    ~DebugLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event &event) override;

private:
    float m_FrameTime;
    const Camera &m_Camera;
};