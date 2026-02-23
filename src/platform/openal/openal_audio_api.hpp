#pragma once

#include "audio/audio_api.hpp"
#include "AL/al.h"
#include "AL/alc.h"

class OpenALAudioAPI : public AudioAPI
{
public:
    virtual void Init() override;
    virtual void Shutdown() override;

    virtual void PlayBackground(const Ref<Audio> &source) override;
    virtual void StopBackground() override;
    virtual AudioAPI::SourceState GetBackgroundState() const override;

    virtual void PlayAt(const Ref<Audio> &source, const glm::vec3 &position) override;

    virtual void SetListenerPosition(glm::vec3 position) override;
    virtual void SetListenerOrientation(glm::vec3 forward, glm::vec3 up) override;

    virtual void SetMasterVolume(float volume) override;
    virtual float GetMasterVolume() const override { return m_MasterVolume; };

    virtual void SetMusicVolume(float volume) override;
    virtual float GetMusicVolume() const override { return m_MusicVolume; };

private:
    void InitBackgroundSource();

private:
    ALCdevice *m_Device;
    ALCcontext *m_Context;

    ALuint m_MusicSource;

    float m_MasterVolume = 1.0f;
    float m_MusicVolume = 1.0f;
};