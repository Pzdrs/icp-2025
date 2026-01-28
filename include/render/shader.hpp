#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>


class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    Shader(): ID(0) {}
    ~Shader() {
        glUseProgram(0);
        glDeleteProgram(ID);
    }
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    unsigned int uniformLoc(const std::string &name) {
        return glGetUniformLocation(ID, name.c_str());
    };
    void use() { glUseProgram(ID); }
};