#pragma once

#include "layer.hpp"

class ImGuiLayer : public Layer
{

public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach();
    void OnDetach();
    void OnUpdate();
    void OnEvent(Event &event);
};