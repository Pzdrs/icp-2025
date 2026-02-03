#pragma once

#include "event.hpp"

class WindowResizeEvent : public Event
{
public:
    WindowResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}

    inline unsigned int getWidth() const { return m_Width; }
    inline unsigned int getHeight() const { return m_Height; }

    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
    }

    EVENT_CLASS_TYPE(WindowResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int m_Width, m_Height;
};

class FramebufferResizeEvent : public Event
{
public:
    FramebufferResizeEvent(unsigned int width, unsigned int height)
        : m_Width(width), m_Height(height) {}

    inline unsigned int getWidth() const { return m_Width; }
    inline unsigned int getHeight() const { return m_Height; }

    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
    }

    EVENT_CLASS_TYPE(FramebufferResize)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
    unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
    WindowCloseEvent() = default;

    EVENT_CLASS_TYPE(WindowClose)
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
};