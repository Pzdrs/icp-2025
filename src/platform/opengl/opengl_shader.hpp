#pragma once

#include "render/shader.hpp"

class OpenGLShader : public Shader
{
public:
    OpenGLShader(const std::string &vertexPath, const std::string &fragmentPath);
    virtual ~OpenGLShader();

    virtual void Bind() const override;
    virtual void Unbind() const override;

    virtual void SetMat4(const std::string &name, const glm::mat4 &value) override;

private:
    uint32_t m_RendererID;
};