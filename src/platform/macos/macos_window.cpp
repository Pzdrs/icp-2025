#include "macos_window.hpp"
#include "event/application_event.hpp"
#include "event/key_event.hpp"
#include "event/mouse_event.hpp"

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char *description)
{
    std::cerr << "GLFW Error (" << error << "): " << description << std::endl;
}

Window *Window::create(const WindowProps &props)
{
    return new MacOSWindow(props);
}

MacOSWindow::MacOSWindow(const WindowProps &props)
{
    init(props);
}

MacOSWindow::~MacOSWindow()
{
    shutdown();
}

void MacOSWindow::init(const WindowProps &props)
{
    m_Data.title = props.title;
    m_Data.width = props.width;
    m_Data.height = props.height;

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        // maybe assert success
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    m_Window = glfwCreateWindow((int)props.width, (int)props.height, m_Data.title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);
    glfwSetWindowUserPointer(m_Window, &m_Data);
    setVSync(true);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                              {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;

        WindowResizeEvent event(width, height);
        data.eventCallback(event); });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                               {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.eventCallback(event); });

    glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                       {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        switch (action)
        {
        case GLFW_PRESS:
        {
            KeyPressedEvent event(key, 0);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            KeyReleasedEvent event(key);
            data.eventCallback(event);
            break;
        }
        case GLFW_REPEAT:
        {
            KeyPressedEvent event(key, 1);
            data.eventCallback(event);
            break;
        }
        } });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                               {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        switch (action)
        {
        case GLFW_PRESS:
        {
            MouseButtonPressedEvent event(button);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE:
        {
            MouseButtonReleasedEvent event(button);
            data.eventCallback(event);
            break;
        }
        } });

    glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset)
                          {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        MouseScrolledEvent event((float)xOffset, (float)yOffset);
        data.eventCallback(event); });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos)
                             {
        WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
        MouseMovedEvent event((float)xPos, (float)yPos);
        data.eventCallback(event); });
}

void *MacOSWindow::getNativeWindow() const
{
    return m_Window;
}

void MacOSWindow::shutdown()
{
    glfwDestroyWindow(m_Window);
}

void MacOSWindow::onUpdate()
{
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void MacOSWindow::setVSync(bool enabled)
{
    glfwSwapInterval(enabled ? 1 : 0);
    m_Data.vSync = enabled;
}

bool MacOSWindow::isVSync() const
{
    return m_Data.vSync;
}