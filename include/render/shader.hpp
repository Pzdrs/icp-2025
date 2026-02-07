#pragma once

#include <glm/glm.hpp>

class Shader
{
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetInt(const std::string &name, int value) = 0;
    virtual void SetMat4(const std::string &name, const glm::mat4 &value) = 0;

    virtual const std::string &GetName() const = 0;

    static Ref<Shader> Create(const std::string &path);
};

class ShaderLibrary
{
public:
    void Add(const std::string &name, const Ref<Shader> &shader);
    void Add(const Ref<Shader> &shader);
    Ref<Shader> Load(const std::string &filepath);
    Ref<Shader> Load(const std::string &name, const std::string &filepath);

    Ref<Shader> Get(const std::string &name);

    bool Exists(const std::string &name) const;

private:
    std::unordered_map<std::string, Ref<Shader>> m_Shaders;
};