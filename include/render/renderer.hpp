#pragma once
#include <camera.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <render/shader.hpp>
#include <render/vertex_array.hpp>

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void drawWithoutIB(const VertexArray &va, const unsigned int count, const Shader &shader) const;
    int init();
    void shutdown();
    void setViewport(int x, int y, int width, int height);

private:
};

enum class RendererAPI
{
    None = 0,
    OpenGL = 1
};

class RendererNew
{
public:
    static inline RendererAPI CurrentAPI() { return s_RendererAPI; }

private:
    static RendererAPI s_RendererAPI;
};
