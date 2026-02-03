#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/renderer.hpp"
#include <stdio.h>
#include <iostream>

int Renderer::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    return 0;
}

void Renderer::setViewport(int x, int y, int width, int height)
{
    glViewport(x, y, width, height);
}

void Renderer::shutdown()
{
    // Cleanup code for the renderer would go here
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const
{
    shader.use();
    va.bind();
    ib.bind();

    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::drawWithoutIB(const VertexArray &va, const unsigned int count, const Shader &shader) const
{
    shader.use();
    va.bind();
    glDrawArrays(GL_TRIANGLES, 0, count);
}