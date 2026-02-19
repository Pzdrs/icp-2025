#pragma once
#include "render/camera/camera.hpp"
#include <glm/glm.hpp>
#include "render/vertex_array.hpp"
#include "render/shader.hpp"

// TODO: add batching support to reduce draw calls
class Renderer3D
{
public:
    static void Init();
    static void Shutdown();

    static void DrawMesh(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform = glm::mat4(1.0f));
};