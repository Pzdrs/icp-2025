#include "render/shader.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <assert.h>

std::string ReadShaderFile(const std::string &filepath)
{
    std::ifstream file(filepath, std::ios::in); // open file for reading
    if (!file.is_open())
    {
        throw std::runtime_error("Failed to open shader file: " + filepath);
    }

    std::stringstream buffer; // create a stringstream
    buffer << file.rdbuf();   // read the entire file into the stream
    return buffer.str();      // return the contents as a string
}

unsigned int CompileShader(unsigned int type, const std::string &source)
{
    unsigned int id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = (char *)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

Shader::Shader(const char *vertexPath, const char *fragmentPath)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, ReadShaderFile(vertexPath));
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, ReadShaderFile(fragmentPath));

    assert(vs != 0 && fs != 0);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    this->ID = program;
}

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
} 
void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}