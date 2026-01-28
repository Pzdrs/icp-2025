#pragma once
#include <GLFW/glfw3.h>

class Window
{
    public:
    void init();
    void shutdown();

    GLFWwindow* window;
};