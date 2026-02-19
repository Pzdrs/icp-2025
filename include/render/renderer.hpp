#pragma once
#include <glm/glm.hpp>
#include "render/shader.hpp"
#include "render/vertex_array.hpp"
#include "renderer_api.hpp"
#include "render/shader.hpp"
#include "render/camera/camera.hpp"

struct Screenshot;

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
    uint32_t VertexCount = 0;
    uint32_t IndexCount = 0;

    void Reset()
    {
        DrawCalls = 0;
        VertexCount = 0;
        IndexCount = 0;
    }
};

class Renderer
{
public:
    struct SceneData
    {
        glm::mat4 ViewProjectionMatrix;
    };

    static void Init();
    static void Shutdown();

    static void OnWindowResize(uint32_t width, uint32_t height);

    static void BeginScene(const Camera &camera);
    static void EndScene();

    static const RendererCapabilities &GetCapabilities() { return s_Capabilities; }
    static const Statistics &GetStats() { return s_Stats; }
    static const SceneData &GetSceneData() { return s_SceneData; }

    static void AddDrawCall(uint32_t count = 1) { s_Stats.DrawCalls += count; }
    static void AddVertices(uint32_t count) { s_Stats.VertexCount += count; }
    static void AddIndices(uint32_t count) { s_Stats.IndexCount += count; }

    static RendererAPI::API CurrentAPI() { return RendererAPI::CurrentAPI(); }
    static Screenshot CaptureScreenshot(const unsigned int width, const unsigned int height);

private:
    inline static SceneData s_SceneData;

    inline static RendererCapabilities s_Capabilities;
    inline static Statistics s_Stats;
};
