#pragma once

#include "event.hpp"

class MouseMovedEvent : public Event
{
public:
    MouseMovedEvent(float x, float y)
        : m_MouseX(x), m_MouseY(y) {}

    inline float getX() const { return m_MouseX; }
    inline float getY() const { return m_MouseY; }

    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_MouseX) + ", " + std::to_string(m_MouseY);
    }

    EVENT_CLASS_TYPE(MouseMoved)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_MouseX, m_MouseY;
};

class MouseScrolledEvent : public Event
{
public:
    MouseScrolledEvent(float xOffset, float yOffset)
        : m_XOffset(xOffset), m_YOffset(yOffset) {}
    inline float getXOffset() const { return m_XOffset; }
    inline float getYOffset() const { return m_YOffset; }

    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_XOffset) + ", " + std::to_string(m_YOffset);
    }

    EVENT_CLASS_TYPE(MouseScrolled)
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
private:
    float m_XOffset, m_YOffset;
};

class MouseButtonEvent : public Event
{
public:
    inline int getMouseButton() const { return m_Button; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
protected:
    MouseButtonEvent(int button)
        : m_Button(button) {}
    int m_Button;
};

class MouseButtonPressedEvent : public MouseButtonEvent
{
public:
    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button) {}
    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_Button);
    }
    EVENT_CLASS_TYPE(MouseButtonPressed)
};

class MouseButtonReleasedEvent : public MouseButtonEvent
{
public:
    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button) {}
    std::string toString() const override
    {
        return std::string(getName()) + ": " + std::to_string(m_Button);
    }
    EVENT_CLASS_TYPE(MouseButtonReleased)
};