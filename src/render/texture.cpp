#include "pch.hpp"
#include "render/texture.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_texture.hpp"
#include "asset/asset_manager.hpp"
#include <stb_image.h>

Ref<Texture2D> Texture2D::Create(const TextureSpecification &spec, Buffer data)
{
    switch (Renderer::CurrentAPI())
    {
    case RendererAPI::API::None:
        return nullptr;
    case RendererAPI::API::OpenGL:
        return CreateRef<OpenGLTexture2D>(spec, data);
    }

    // assert
    return nullptr;
}

Ref<Texture2D> TextureImporter::ImportTexture2D(const AssetHandle handle, const std::filesystem::path &path)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    Buffer data;
    data.Data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

    if (data.Data == nullptr)
    {
        std::cout << "Failed to load texture: " << path << std::endl;
        return nullptr;
    }

    TextureSpecification spec{
        .Width = (uint32_t)width,
        .Height = (uint32_t)height};

    switch (nrChannels)
    {
    case 3:
        spec.Format = ImageFormat::RGB8;
        data.Size = width * height * 3;
        break;
    case 4:
        spec.Format = ImageFormat::RGBA8;
        data.Size = width * height * 4;
        break;
    }

    Ref<Texture2D> texture = Texture2D::Create(spec, data);
    texture->SetHandle(handle);

    data.Release();
    return texture;
}