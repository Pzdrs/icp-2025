#include "pch.hpp"

#include "openal_audio_api.hpp"
#include "openal_audio.hpp"
#include <memory>

void OpenALAudioAPI::Init()
{
    m_Device = alcOpenDevice(nullptr);
    if (!m_Device)
    {
        LOG("Failed to open OpenAL device!");
        return;
    }

    m_Context = alcCreateContext(m_Device, nullptr);
    if (!m_Context)
    {
        LOG("Failed to create OpenAL context!");
        alcCloseDevice(m_Device);
        return;
    }

    if (!alcMakeContextCurrent(m_Context))
    {
        LOG("Failed to make OpenAL context current!");
        alcDestroyContext(m_Context);
        alcCloseDevice(m_Device);
        return;
    }

    LOG("OpenAL initialized successfully");
}

void OpenALAudioAPI::Shutdown()
{
    alcMakeContextCurrent(nullptr);

    if (m_Context)
        alcDestroyContext(m_Context);

    if (m_Device)
        alcCloseDevice(m_Device);
}

// do source pools later
void OpenALAudioAPI::Play(const Ref<Audio> &source)
{
    // Do we even need to check dynamic cast?
    auto openALAudio = std::dynamic_pointer_cast<OpenALAudio>(source);
    if (!openALAudio)
    {
        LOG("OpenALAudioAPI::Play received non-OpenAL audio asset");
        return;
    }

    ALuint sourceID;
    alGenSources(1, &sourceID);

    // 2D sound
    alSourcei(sourceID, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSourcef(sourceID, AL_GAIN, 1.0f);
    alSourcei(sourceID, AL_LOOPING, AL_TRUE);

    // Attach decoded OpenAL buffer
    alSourcei(sourceID, AL_BUFFER, static_cast<ALint>(openALAudio->GetHandle()));

    alSourcePlay(sourceID);
}
