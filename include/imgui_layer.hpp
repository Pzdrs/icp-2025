#pragma once

#include "layer.hpp"

class ImGuiLayer : public Layer
{

public:
    ImGuiLayer();
    ~ImGuiLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnEvent(Event &event) override;

    void Begin();
    void End();
};