#include "pch.hpp"
#include "opengl_texture.hpp"

static GLenum ImageFormatToGLDataFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::R8:
        return GL_RED;
    case ImageFormat::RGB8:
        return GL_RGB;
    case ImageFormat::RGBA8:
        return GL_RGBA;
    case ImageFormat::RGBA32F:
        return GL_RGBA;
    default:
        return GL_RGBA;
    }
}

static GLenum ImageFormatToGLInternalFormat(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::R8:
        return GL_R8;
    case ImageFormat::RGB8:
        return GL_RGB8;
    case ImageFormat::RGBA8:
        return GL_RGBA8;
    case ImageFormat::RGBA32F:
        return GL_RGBA32F;
    default:
        return GL_RGBA8;
    }
}

static GLenum ImageFormatToGLDataType(ImageFormat format)
{
    switch (format)
    {
    case ImageFormat::RGBA32F:
        return GL_FLOAT;
    default:
        return GL_UNSIGNED_BYTE;
    }
}

OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification &spec, Buffer data)
    : m_Specification(spec), m_Width(spec.Width), m_Height(spec.Height)
{
    glGenTextures(1, &m_RendererID);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, spec.GenerateMips ? GL_NEAREST_MIPMAP_NEAREST : GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    if (data)
    {
        SetData(data);

        if (spec.GenerateMips)
            glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void OpenGLTexture2D::SetData(Buffer data)
{
    GLenum internalFormat = ImageFormatToGLInternalFormat(m_Specification.Format);
    GLenum dataFormat = ImageFormatToGLDataFormat(m_Specification.Format);
    GLenum dataType = ImageFormatToGLDataType(m_Specification.Format);

    glBindTexture(GL_TEXTURE_2D, m_RendererID);

    if (dataFormat == GL_RGB || dataFormat == GL_RED)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, dataFormat, dataType, data.Data);

    if (dataFormat == GL_RGB || dataFormat == GL_RED)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_RendererID);
}

void OpenGLTexture2D::Bind(uint32_t slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_RendererID);
}