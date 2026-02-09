#include "pch.hpp"
#include "render/sub_texture.hpp"

SubTexture2D::SubTexture2D(const Ref<Texture2D> &texture, const glm::vec2 &min, const glm::vec2 &max)
    : m_Texture(texture)
{
    // min = bottom-left, max = top-right
    m_TexCoords[0] = {min.x, min.y};
    m_TexCoords[1] = {max.x, min.y};
    m_TexCoords[2] = {max.x, max.y};
    m_TexCoords[3] = {min.x, max.y};
}

Ref<SubTexture2D> SubTexture2D::CreateFromCoordinates(const Ref<Texture2D> &texture, const glm::vec2 &coords, const glm::vec2 &spriteSize)
{
    glm::vec2 min = {(coords.x * spriteSize.x) / texture->GetWidth(), (coords.y * spriteSize.y) / texture->GetHeight()};
    glm::vec2 max = {((coords.x + 1) * spriteSize.x) / texture->GetWidth(), ((coords.y + 1) * spriteSize.y) / texture->GetHeight()};
    return CreateRef<SubTexture2D>(texture, min, max);
}