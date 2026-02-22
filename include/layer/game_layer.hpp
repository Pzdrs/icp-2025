#pragma once

#include "layer/layer.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "render/renderer.hpp"
#include "render/camera/camera_controller.hpp"
#include "world.hpp"
#include "block.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"

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

    Camera &GetCamera() { return m_CameraController.GetCamera(); }

private:
    bool OnKeyPressed(KeyPressedEvent &e);

    void SetPaused(bool paused);

private:
    bool m_Paused = false;
    bool m_Fullscreen = false;

    CreativeCameraController m_CameraController;
    AssetHandle m_BlockAtlasHandle;
    AssetHandle m_SoundtrackHandle;
    World m_World;
    ShaderLibrary m_ShaderLibrary;
};