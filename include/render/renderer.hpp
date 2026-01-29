#pragma once
#include <camera.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <render/shader.hpp>
#include <vertex_array.hpp>
#include <index_buffer.hpp>

class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray &va, const IndexBuffer &ib, const Shader &shader);
    int init();
    void shutdown();

private:
};