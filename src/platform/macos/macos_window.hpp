#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "window.hpp"
#include <render/graphics_context.hpp>


class MacOSWindow : public Window
{
public:
    MacOSWindow(const WindowProps &props);
    virtual ~MacOSWindow();

    void onUpdate() override;

    unsigned int getWidth() const override { return m_Data.width; };
    unsigned int getHeight() const override { return m_Data.height; };

    unsigned int getFramebufferWidth() const override { return m_Data.fbWidth; };
    unsigned int getFramebufferHeight() const override { return m_Data.fbHeight; };

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
    GraphicsContext *m_Context;

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