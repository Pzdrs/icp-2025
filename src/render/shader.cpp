#include "pch.hpp"
#include "render/shader.hpp"
#include "render/renderer.hpp"
#include "../platform/opengl/opengl_shader.hpp"

Ref<Shader> Shader::Create(const std::string &path)
{
	switch (Renderer::CurrentAPI())
	{
	case RendererAPI::API::None:
		return nullptr;
	case RendererAPI::API::OpenGL:
		return CreateRef<OpenGLShader>(path);
	}
	return nullptr;
}

void ShaderLibrary::Add(const std::string &name, const Ref<Shader> &shader)
{
	m_Shaders[name] = shader;
}

void ShaderLibrary::Add(const Ref<Shader> &shader)
{
	auto &name = shader->GetName();
	Add(name, shader);
}

Ref<Shader> ShaderLibrary::Load(const std::string &filepath)
{
	auto shader = Shader::Create(filepath);
	Add(shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Load(const std::string &name, const std::string &filepath)
{
	auto shader = Shader::Create(filepath);
	Add(name, shader);
	return shader;
}

Ref<Shader> ShaderLibrary::Get(const std::string &name)
{
	if (!Exists(name))
	{
		throw std::runtime_error("Shader not found: " + name);
	}
	return m_Shaders[name];
}

bool ShaderLibrary::Exists(const std::string &name) const
{
	return m_Shaders.find(name) != m_Shaders.end();
}
