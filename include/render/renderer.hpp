#pragma once
#include <camera.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <render/shader.hpp>
#include <render/vertex_array.hpp>
#include <render/index_buffer.hpp>

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader) const;
    void drawWithoutIB(const VertexArray &va, const unsigned int count, const Shader &shader) const;
    int init();
    void shutdown();

private:
};