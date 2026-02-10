#pragma once
#include "event/event.hpp"

struct WindowProps
{
    unsigned int width;
    unsigned int height;
    std::string title;

    WindowProps(unsigned int w = 1280, unsigned int h = 720, const std::string &t = "Application")
        : width(w), height(h), title(t) {}
};

class Window
{
public:
    using EventCallbackFunc = std::function<void(Event &)>;

    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;
    virtual unsigned int GetFramebufferWidth() const = 0;
    virtual unsigned int GetFramebufferHeight() const = 0;

    virtual void SetEventCallback(const EventCallbackFunc &callback) = 0;
    virtual void SetVSync(bool enabled) = 0;
    virtual void SetMouseLocked(bool locked) = 0; 
    virtual bool IsVSync() const = 0;
    virtual void SetFullscreen(bool fullscreen) = 0;

    virtual void *GetNativeWindow() const = 0;

    static Window *Create(const WindowProps &props = WindowProps());
};