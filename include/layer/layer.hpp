#pragma once

#include "event/event.hpp"

class Layer
{
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer() = default;

    virtual void OnAttach() {};
    virtual void OnDetach() {};
    virtual void OnUpdate(float dt) {};
    virtual void OnUpdateFixed(float dt) {};
    virtual void OnImGuiRender() {};
    virtual void OnEvent(Event &event) {};

    inline const std::string &GetName() const { return m_DebugName; }

protected:
    std::string m_DebugName;
};