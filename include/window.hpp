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

    void lockMouse();
    void unlockMouse();
    void setVSync(bool enabled);

    GLFWwindow* window;
};