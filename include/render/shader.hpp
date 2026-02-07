#pragma once

#include <glm/glm.hpp>

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

    static Ref<Shader> Create(const std::string &vertexPath, const std::string &fragmentPath);
};