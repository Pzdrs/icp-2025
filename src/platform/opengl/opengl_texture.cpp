#include "pch.hpp"
#include "opengl_texture.hpp"

OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification &spec, Buffer data)
    : m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data)
        SetData(data);
}

void OpenGLTexture2D::SetData(Buffer data)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.Data);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glBindTextureUnit(slot, m_RendererID);
}