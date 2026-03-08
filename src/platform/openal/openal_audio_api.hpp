#pragma once

#include "audio/audio_api.hpp"
#include "AL/al.h"
#include "AL/alc.h"

// TODO: implement audio streaming for music to avoid loading entire tracks into memory
class OpenALAudioAPI : public AudioAPI
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;
    virtual void Update() override;

    virtual void PlayMusic(const Ref<Audio> &source) override;
    virtual void StopMusic() override;
    virtual AudioAPI::SourceState GetMusicState() const override;

    virtual void PlayAt(const Ref<Audio> &source, const glm::vec3 &position) override;

    virtual void SetListenerPosition(glm::vec3 position) override;
    virtual void SetListenerOrientation(glm::vec3 forward, glm::vec3 up) override;

    virtual void SetMasterVolume(float volume) override;
    virtual float GetMasterVolume() const override { return m_MasterVolume; };

    virtual void SetMusicVolume(float volume) override;
    virtual float GetMusicVolume() const override { return m_MusicVolume; };

private:
    void InitSources();
    ALuint AcquireSource();

private:
    struct PooledSource
    {
        ALuint handle;
        bool inUse = false;
    };

private:
    ALCdevice *m_Device;
    ALCcontext *m_Context;

    ALuint m_MusicSource;

    std::array<PooledSource, 32> m_SourcePool;

    float m_MasterVolume = 1.0f;
    float m_MusicVolume = 1.0f;
};