#pragma once

#include "audio/audio.hpp"
#include "AL/al.h"

class OpenALAudio : public Audio
{
public:
    OpenALAudio(const void *data, uint32_t size, uint32_t sampleRate, uint32_t channels);
    virtual ~OpenALAudio();

    ALuint GetHandle() const { return m_Handle; }
private:
    ALuint m_Handle;
};
