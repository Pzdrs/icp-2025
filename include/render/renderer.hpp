#pragma once
#include <camera.hpp>
#include <glm/glm.hpp>
#include "render/shader.hpp"
#include "render/vertex_array.hpp"
#include "renderer_api.hpp"

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene();
    static void EndScene();

    static void Submit(const std::shared_ptr<VertexArray> &vertexArray);

    static RendererAPI::API CurrentAPI() { return RendererAPI::CurrentAPI(); }
};
