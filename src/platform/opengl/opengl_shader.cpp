#include "pch.hpp"
#include "opengl_shader.hpp"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

std::string ReadShaderFile(const std::string &filepath);
unsigned int CompileShader(unsigned int type, const std::string &source);

OpenGLShader::OpenGLShader(const std::string &vertexPath, const std::string &fragmentPath)
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

    m_RendererID = program;
}

OpenGLShader::~OpenGLShader()
{
    glUseProgram(0);
    glDeleteProgram(m_RendererID);
}

void OpenGLShader::Bind() const
{
    glUseProgram(m_RendererID);
}

void OpenGLShader::Unbind() const
{
    glUseProgram(0);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

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
