#pragma once

#include <glm/glm.hpp>
#include "vertex_array.hpp"

struct RendererCapabilities;

class RendererAPI
{
public:
    enum class API
    {
        None = 0,
        OpenGL = 1
    };

public:
    virtual ~RendererAPI() = default;

    virtual void Init() = 0;
    virtual RendererCapabilities &GetCapabilities() = 0;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
    virtual void SetClearColor(const glm::vec4 &color) = 0;
    virtual void SetMultisampling(bool enabled) = 0;
    virtual bool IsMultisamplingEnabled() const = 0;
    virtual void Clear() = 0;

    virtual void DrawIndexed(const Ref<VertexArray> &vertexArray, uint32_t indexCount = 0) = 0;

    virtual std::vector<unsigned char> CaptureScreenshot(const unsigned int width, const unsigned int height) = 0;

    static API CurrentAPI() { return s_API; }
    static Scope<RendererAPI> Create();

private:
    static API s_API;
};