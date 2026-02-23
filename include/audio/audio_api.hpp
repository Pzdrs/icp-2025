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

    enum class SourceState {
        Stopped = 0,
        Playing,
        Paused
    };

public:
    virtual ~AudioAPI() = default;

    virtual void Init() = 0;
    virtual void Shutdown() = 0;

    virtual void PlayMusic(const Ref<Audio> &source) = 0;
    virtual void StopMusic() = 0;
    virtual SourceState GetMusicState() const = 0;
    
    virtual void PlayAt(const Ref<Audio> &source, const glm::vec3 &position) = 0;

    virtual void SetListenerPosition(glm::vec3 position) = 0;
    virtual void SetListenerOrientation(glm::vec3 forward, glm::vec3 up) = 0;

    virtual void SetMasterVolume(float volume) = 0;
    virtual float GetMasterVolume() const = 0;

    virtual void SetMusicVolume(float volume) = 0;
    virtual float GetMusicVolume() const = 0;

    static API CurrentAPI() { return s_API; }
    static Scope<AudioAPI> Create();

private:
    static API s_API;
};
