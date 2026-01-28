#pragma once
#include <GLFW/glfw3.h>

class Window
{
    public:
    void init();
    void shutdown();
    void pollEvents() { glfwPollEvents(); }
    void swapBuffers() { glfwSwapBuffers(window); }
    bool shouldClose() { return glfwWindowShouldClose(window); }

    GLFWwindow* window;
};