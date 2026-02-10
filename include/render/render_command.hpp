#pragma once
#include "renderer_api.hpp"

class RenderCommand
{
public:
    inline static void Init()
    {
        s_RendererAPI->Init();
    }

    inline static RendererCapabilities &GetCapabilities()
    {
        return s_RendererAPI->GetCapabilities();
    }

    inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
    {
        s_RendererAPI->SetViewport(x, y, width, height);
    }

    inline static void SetClearColor(const glm::vec4 &color)
    {
        s_RendererAPI->SetClearColor(color);
    }

    inline static void Clear()
    {
        s_RendererAPI->Clear();
    }
    inline static void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0)
    {
        s_RendererAPI->DrawIndexed(vertexArray, indexCount);
    }

    inline static std::vector<unsigned char> CaptureScreenshot(const unsigned int width, const unsigned int height)
    {
        return s_RendererAPI->CaptureScreenshot(width, height);
    }
private:
    static Scope<RendererAPI> s_RendererAPI;
};