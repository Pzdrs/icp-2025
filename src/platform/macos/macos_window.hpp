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

    void OnUpdate() override;

    unsigned int GetWidth() const override { return m_Data.width; };
    unsigned int GetHeight() const override { return m_Data.height; };

    unsigned int GetFramebufferWidth() const override { return m_Data.fbWidth; };
    unsigned int GetFramebufferHeight() const override { return m_Data.fbHeight; };

    inline void SetEventCallback(const EventCallbackFunc &callback) override { m_Data.eventCallback = callback; };
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;
    void SetMouseLocked(bool locked) override;
    void SetFullscreen(bool fullscreen) override;

    void *GetNativeWindow() const override;

private:
    virtual void Init(const WindowProps &props);
    virtual void Shutdown();

private:
    GLFWwindow *m_Window;
    GraphicsContext *m_Context;

    struct WindowData
    {
        std::string title;
        unsigned int width, height;
        unsigned int fbWidth, fbHeight;
        bool vSync;

        int windowedX = 0;
        int windowedY = 0;
        unsigned int windowedWidth = 0;
        unsigned int windowedHeight = 0;

        EventCallbackFunc eventCallback;
    };

    WindowData m_Data;
};