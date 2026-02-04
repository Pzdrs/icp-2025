#include "macos_input.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "scuffcraft.hpp"

Input *Input::s_Instance = new MacOSInput();

bool MacOSInput::IsKeyPressedImpl(int keycode)
{
    auto window = static_cast<GLFWwindow *>(Scuffcraft::get().getWindow().getNativeWindow());
    int state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool MacOSInput::IsMouseButtonPressedImpl(int buttoncode)
{
    auto window = static_cast<GLFWwindow *>(Scuffcraft::get().getWindow().getNativeWindow());
    int state = glfwGetMouseButton(window, buttoncode);
    return state == GLFW_PRESS;
}

std::pair<float, float> MacOSInput::GetMousePositionImpl()
{
    auto window = static_cast<GLFWwindow *>(Scuffcraft::get().getWindow().getNativeWindow());
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    return {(float)xpos, (float)ypos};
}

float MacOSInput::GetMouseXImpl()
{
    auto [x, y] = GetMousePositionImpl();
    return x;
}

float MacOSInput::GetMouseYImpl()
{
    auto [x, y] = GetMousePositionImpl();
    return y;
}