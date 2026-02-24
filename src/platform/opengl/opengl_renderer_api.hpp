#pragma once

#include "render/renderer_api.hpp"
#include <render/renderer.hpp>

class OpenGLRendererAPI : public RendererAPI
{
public:
    virtual void Init() override;
    virtual RendererCapabilities &GetCapabilities() override;
    virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
    virtual void SetMultisampling(bool enabled) override;
    virtual bool IsMultisamplingEnabled() const override { return m_MultisamplingEnabled; }

    virtual void SetClearColor(const glm::vec4 &color) override;
    virtual void Clear() override;

    virtual void DrawIndexed(const Scope<VertexArray> &vertexArray, uint32_t indexCount = 0) override;

    virtual std::vector<unsigned char> CaptureScreenshot(const unsigned int width, const unsigned int height) override;
private:
    bool m_MultisamplingEnabled = false;
};