#pragma once

#include "layer.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "camera.hpp"
#include "world.hpp"
#include "block.hpp"
#include "render/shader.hpp"

static const std::string BLOCK_MANIFEST = "resources/blocks.json";
static const std::string BLOCK_ATLAS = "resources/blocks.png";

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
    bool onMouseMove(MouseMovedEvent &e);
    bool onScroll(MouseScrolledEvent &e);
    bool onWindowResize(WindowResizeEvent &e);
    void processInput(float dt);

private:
    Renderer m_Renderer;
    Camera m_Camera; 
    Shader m_Shader;
    BlockRegistry m_BlockRegistry;
    World m_World;
};