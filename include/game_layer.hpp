#pragma once

#include "layer.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "render/renderer.hpp"

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
private:
};