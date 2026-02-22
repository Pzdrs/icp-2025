#include "pch.hpp"
#include "openal_audio.hpp"

static ALenum GetOpenALFormat(uint32_t channels)
{
    switch (channels)
    {
    case 1:
        return AL_FORMAT_MONO16;
    case 2:
        return AL_FORMAT_STEREO16;
    }
    // assert
    return 0;
}

OpenALAudio::OpenALAudio(const void *data, uint32_t size, uint32_t sampleRate, uint32_t channels)
{
    alGenBuffers(1, &m_Handle);
    alBufferData(m_Handle, GetOpenALFormat(channels), data, size, sampleRate);
}

OpenALAudio::~OpenALAudio()
{
    alDeleteBuffers(1, &m_Handle);
}
