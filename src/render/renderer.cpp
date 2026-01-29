#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/renderer.hpp"
#include <stdio.h>

int Renderer::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    glEnable(GL_DEPTH_TEST);
    return 0;
}

void Renderer::shutdown()
{
    // Cleanup code for the renderer would go here
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader)
{
    shader.use();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}