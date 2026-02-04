#pragma once

#include "render/graphics_context.hpp"

struct GLFWwindow;

class OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow *windowHandle);

    virtual void Init() override;
    virtual void SwapBuffers() override;

private:
    GLFWwindow *m_WindowHandle;
};