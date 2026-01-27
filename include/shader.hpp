#pragma once

#include <string>
#include <GL/glew.h>

class Shader
{
public:
    unsigned int ID;

    Shader(const char *vertexPath, const char *fragmentPath);
    ~Shader() {
        glUseProgram(0);
        glDeleteProgram(ID);
    }
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void use() { glUseProgram(ID); }
};