#pragma once
#include "render/camera/camera.hpp"
#include <glm/glm.hpp>
#include "render/vertex_array.hpp"
#include "render/shader.hpp"
#include "mesh.hpp"
#include "render/material.hpp"

// TODO: add batching support to reduce draw calls
class Renderer3D
{
public:
    struct Statistics
    {
        uint32_t TriangleCount;
        uint32_t QuadCount;

        void Reset()
        {
            TriangleCount = 0;
            QuadCount = 0;
        }
    };

    static void Init();
    static void Shutdown();

    static void DrawMesh(const Scope<VertexArray> &vertexArray, const Ref<Material> &material, const glm::mat4 &transform = glm::mat4(1.0f));

    static const Statistics &GetStats() { return s_Stats; }
    static void ResetStats() { s_Stats.Reset(); }

private:
    inline static Statistics s_Stats{};
};