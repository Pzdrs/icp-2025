#pragma once
#include <string>
#include <functional>

enum class EventType
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    WindowMoved,
    KeyPressed,
    KeyReleased,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseMoved,
    MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryApplication = 1 << 0,
    EventCategoryInput = 1 << 1,
    EventCategoryKeyboard = 1 << 2,
    EventCategoryMouse = 1 << 3,
    EventCategoryMouseButton = 1 << 4
};

#define EVENT_CLASS_TYPE(type)                                                  \
    static EventType getStaticType() { return EventType::type; }              \
    virtual EventType getEventType() const override { return getStaticType(); } \
    virtual const char *getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual int getCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatcher;

public:
    virtual EventType getEventType() const = 0;
    virtual const char *getName() const = 0;
    virtual int getCategoryFlags() const = 0;
    virtual std::string toString() const { return getName(); }
    inline bool isInCategory(EventCategory category)
    {
        return getCategoryFlags() & category;
    }

protected:
    bool m_Handled = false;
};

class EventDispatcher
{
    template <typename T>
    using EventFunc = std::function<bool(T &)>;

public:
    EventDispatcher(Event &event)
        : m_Event(event) {}

    template <typename T>
    bool dispatch(EventFunc<T> func) {
        if (m_Event.getEventType() == T::getStaticType()) {
            m_Event.m_Handled = func(*(T *)&m_Event);
            return true;
        }
        return false;
    }
private:
    Event &m_Event;
};