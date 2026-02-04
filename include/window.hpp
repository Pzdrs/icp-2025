#pragma once
#include "event/event.hpp"
#include <functional>

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

    virtual void onUpdate() = 0;

    virtual unsigned int getWidth() const = 0;
    virtual unsigned int getHeight() const = 0;

    virtual void setEventCallback(const EventCallbackFunc &callback) = 0;
    virtual void setVSync(bool enabled) = 0;
    virtual void setMouseLocked(bool locked) = 0; 
    virtual bool isVSync() const = 0;

    virtual void *getNativeWindow() const = 0;

    static Window *create(const WindowProps &props = WindowProps());
};