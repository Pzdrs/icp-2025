#pragma once

#include "layer.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "render/renderer.hpp"
#include "camera_controller.hpp"
#include "world.hpp"
#include "block.hpp"
#include "render/shader.hpp"

class GameLayer : public Layer
{
public:
    GameLayer();
    ~GameLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event &event) override;

private:
    FreeCameraController m_CameraController;
    BlockRegistry m_BlockRegistry;
    World m_World;
    Shader m_Shader;
};