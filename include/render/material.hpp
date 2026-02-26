#pragma once

#include <glm/glm.hpp>
#include "core.hpp"
#include "render/shader.hpp"
#include "render/texture.hpp"

class Material
{
public:
    Material(Ref<Shader> shader);
    virtual ~Material() = default;

    void Bind() const;

    void SetTexture(Ref<Texture> texture) { m_Texture = texture; }

    Ref<Shader> GetShader() const { return m_Shader; }

private:
    Ref<Shader> m_Shader;
    Ref<Texture> m_Texture;
};