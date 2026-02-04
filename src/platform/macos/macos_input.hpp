#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "input.hpp"

class MacOSInput : public Input
{
protected:
    virtual bool IsKeyPressedImpl(int keycode) override;
    virtual bool IsMouseButtonPressedImpl(int buttoncode) override;
    virtual std::pair<float, float> GetMousePositionImpl() override;
    virtual float GetMouseXImpl() override;
    virtual float GetMouseYImpl() override;
};