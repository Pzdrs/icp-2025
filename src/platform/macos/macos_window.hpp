#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "window.hpp"

class MacOSWindow : public Window
{
public:
    MacOSWindow(const WindowProps &props);
    virtual ~MacOSWindow();

    void onUpdate() override;

    unsigned int getWidth() const override { return m_Data.width; };
    unsigned int getHeight() const override { return m_Data.height; };

    inline void setEventCallback(const EventCallbackFunc &callback) override { m_Data.eventCallback = callback; };
    void setVSync(bool enabled) override;
    bool isVSync() const override;
    void setMouseLocked(bool locked) override;

    void *getNativeWindow() const override;
private:
    virtual void init(const WindowProps &props);
    virtual void shutdown();
private:
    GLFWwindow *m_Window;

    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        unsigned int fbWidth, fbHeight;
        bool vSync;

        EventCallbackFunc eventCallback;
    };

    WindowData m_Data;
};