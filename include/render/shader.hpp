#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    Shader(const std::string &vertexPath, const std::string &fragmentPath);
    ~Shader()
    {
        glUseProgram(0);
        glDeleteProgram(m_RendererID);
    }
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    void use() const { glUseProgram(m_RendererID); }

private:
    uint32_t m_RendererID;
};