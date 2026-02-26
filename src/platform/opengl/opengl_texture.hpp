#pragma once

#include "render/texture.hpp"
#include <string>
#include <glad/glad.h>

class OpenGLTexture2D : public Texture2D
{
public:
    OpenGLTexture2D(const TextureSpecification &spec, Buffer data);
    virtual ~OpenGLTexture2D();

    virtual uint32_t GetWidth() const override { return m_Width; }
    virtual uint32_t GetHeight() const override { return m_Height; }
    virtual uint32_t GetRendererID() const override { return m_RendererID; }

    virtual const TextureSpecification &GetSpecification() const override { return m_Specification; }

    virtual void SetData(Buffer data) override;

    virtual void Bind(uint32_t slot = 0) const override;

    virtual bool operator==(const Texture &other) const override
    {
        return m_RendererID == other.GetRendererID();
    }

private:
    TextureSpecification m_Specification;

    bool m_IsLoaded = false;
    uint32_t m_Width, m_Height;
    uint32_t m_RendererID;
};