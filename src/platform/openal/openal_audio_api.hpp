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

private:
    void InitBackgroundSource();

private:
    ALCdevice *m_Device;
    ALCcontext *m_Context;

    ALuint m_BackgroundSource;
};