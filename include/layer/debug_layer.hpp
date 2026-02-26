#pragma once

#include "layer/layer.hpp"
#include "render/camera/camera.hpp"
#include "fps_counter.hpp"

class DebugLayer : public Layer
{
public:
    DebugLayer(const Camera &camera);

    virtual void OnImGuiRender() override;

private:
    const Camera &m_Camera;
    FPSCounter m_FPSCounter;
};  