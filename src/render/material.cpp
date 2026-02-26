#include "pch.hpp"
#include "render/material.hpp"

Material::Material(Ref<Shader> shader)
    : m_Shader(shader)
{
}

void Material::Bind() const
{
    m_Shader->Bind();
    m_Texture->Bind();
}