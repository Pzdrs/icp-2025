#include "pch.hpp"
#include "opengl_shader.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace Utils
{
    static GLenum ShaderTypeFromString(const std::string &type)
    {
        if (type == "vertex")
            return GL_VERTEX_SHADER;
        if (type == "fragment" || type == "pixel")
            return GL_FRAGMENT_SHADER;
        std::cerr << "Unknown shader type: " << type << std::endl;
        return 0;
    }
}

OpenGLShader::OpenGLShader(const std::string &path)
{
    std::string source = ReadFile(path);
    auto shaderSources = PreProcess(source);

    CompileAndLink(shaderSources);

    auto lastSlash = path.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = path.rfind('.');
    auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
    m_Name = path.substr(lastSlash, count);
    m_FilePath = path;
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

void OpenGLShader::SetInt(const std::string &name, int value)
{
    glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void OpenGLShader::SetMat4(const std::string &name, const glm::mat4 &value)
{
    glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
std::string OpenGLShader::ReadFile(const std::string &filepath)
{
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (!in.is_open())
    {
        std::cerr << "Failed to open shader file: " << filepath << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string &source)
{
    std::unordered_map<GLenum, std::string> shaderSources;

    const char *typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = source.find(typeToken, 0);
    while (pos != std::string::npos)
    {
        size_t eol = source.find_first_of("\r\n", pos);
        size_t begin = pos + typeTokenLength + 1;
        std::string type = source.substr(begin, eol - begin);

        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        pos = source.find(typeToken, nextLinePos);

        shaderSources[Utils::ShaderTypeFromString(type)] =
            (pos == std::string::npos) ? source.substr(nextLinePos) : source.substr(nextLinePos, pos - nextLinePos);
    }

    return shaderSources;
}

void OpenGLShader::CompileAndLink(const std::unordered_map<GLenum, std::string> &shaderSources)
{
    GLuint program = glCreateProgram();
    std::vector<GLuint> shaderIDs;

    for (auto &[type, src] : shaderSources)
    {
        GLuint shader = glCreateShader(type);
        const char *srcCStr = src.c_str();
        glShaderSource(shader, 1, &srcCStr, nullptr);
        glCompileShader(shader);

        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            GLint logLength;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
            std::vector<GLchar> infoLog(logLength);
            glGetShaderInfoLog(shader, logLength, nullptr, infoLog.data());
            std::cerr << "Shader compilation failed: " << infoLog.data() << std::endl;
            glDeleteShader(shader);
            continue;
        }

        glAttachShader(program, shader);
        shaderIDs.push_back(shader);
    }

    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (!isLinked)
    {
        GLint maxLength;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
        std::vector<GLchar> infoLog(maxLength);
        glGetProgramInfoLog(program, maxLength, &maxLength, infoLog.data());
        std::cerr << "Shader linking failed:\n"
                  << infoLog.data() << std::endl;
        glDeleteProgram(program);
        for (auto id : shaderIDs)
            glDeleteShader(id);
        return;
    }

    for (auto id : shaderIDs)
    {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    m_RendererID = program;
}