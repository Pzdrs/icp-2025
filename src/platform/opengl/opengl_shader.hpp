#pragma once

#include "render/shader.hpp"
#include <glad/glad.h>

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string &path);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetInt(const std::string &name, int value) override;
    virtual void SetMat4(const std::string &name, const glm::mat4 &value) override;

    virtual const std::string &GetName() const override { return m_Name; }

private:
    std::string ReadFile(const std::string &filepath);
    std::unordered_map<GLenum, std::string> PreProcess(const std::string &source);

    void CompileAndLink(const std::unordered_map<GLenum, std::string> &shaderSources);

private:
    uint32_t m_RendererID;
    std::string m_FilePath, m_Name;
};