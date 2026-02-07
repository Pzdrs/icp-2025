#pragma once
#include <glm/glm.hpp>
#include "render/shader.hpp"
#include "render/vertex_array.hpp"
#include "renderer_api.hpp"
#include "render/shader.hpp"
#include "render/camera/camera.hpp"

struct RendererCapabilities
{
    std::string GraphicsAPI;

    std::string Vendor;
    std::string DeviceName;

    std::string DriverVersion;
    std::string ShaderLanguage;

    int APIMajor = 0;
    int APIMinor = 0;
};

struct Statistics
{
    uint32_t DrawCalls = 0;
    uint32_t TriangleCount = 0;

    uint32_t GetTotalVertexCount() const { return TriangleCount * 3; }
    uint32_t GetTotalIndexCount() const { return TriangleCount * 3; }

    void Reset()
    {
        DrawCalls = 0;
        TriangleCount = 0;
    }
};

class Renderer
{
public:
    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const Camera &camera);
    static void EndScene();

    static void Submit(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform = glm::mat4(1.0f));

    static RendererAPI::API CurrentAPI() { return RendererAPI::CurrentAPI(); }

    static const RendererCapabilities &GetCapabilities() { return s_Capabilities; }
    static const Statistics &GetStats() { return s_Stats; }

private:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static Scope<SceneData> s_SceneData;

    static RendererCapabilities s_Capabilities;
    static Statistics s_Stats;
};
