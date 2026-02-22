#pragma once

#include "core.hpp"

class AudioAPI
{
public:
    enum class API
    {
        None = 0,
        OpenAL
    };

public:
    virtual ~AudioAPI() = default;

    virtual void Init() = 0;

    static API CurrentAPI() { return s_API; }
    static Scope<AudioAPI> Create();

private:
    static API s_API;
};
