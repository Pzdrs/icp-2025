#pragma once

#include "core.hpp"
#include "audio/audio.hpp"

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
    virtual void Shutdown() = 0;

    virtual void PlayBackground(const Ref<Audio> &source) = 0;

    virtual void SetMasterVolume(float volume) = 0;
    virtual float GetMasterVolume() const = 0;

    virtual void SetMusicVolume(float volume) = 0;
    virtual float GetMusicVolume() const = 0;

    static API CurrentAPI() { return s_API; }
    static Scope<AudioAPI> Create();

private:
    static API s_API;
};
