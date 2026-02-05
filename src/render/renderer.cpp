#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include "render/renderer.hpp"
#include <stdio.h>
#include <iostream>
#include <render/buffer.hpp>

RendererAPI RendererNew::s_RendererAPI = RendererAPI::OpenGL;

int Renderer::init()
{
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
    va.Bind();
    ib.Bind();

    glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}
void Renderer::drawWithoutIB(const VertexArray &va, const unsigned int count, const Shader &shader) const
{
    shader.use();
    va.Bind();
    glDrawArrays(GL_TRIANGLES, 0, count);
}