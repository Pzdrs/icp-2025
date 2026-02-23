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

    InitBackgroundSource();

    LOG("OpenAL initialized successfully");
}

void OpenALAudioAPI::InitBackgroundSource()
{
    alGenSources(1, &m_BackgroundSource);

    // 2D sound
    alSourcei(m_BackgroundSource, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(m_BackgroundSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSourcef(m_BackgroundSource, AL_GAIN, 1.0f);
}

void OpenALAudioAPI::Shutdown()
{
    alcMakeContextCurrent(nullptr);

    if (m_Context)
        alcDestroyContext(m_Context);

    if (m_Device)
        alcCloseDevice(m_Device);
}

void OpenALAudioAPI::PlayBackground(const Ref<Audio> &source)
{
    auto openALAudio = std::dynamic_pointer_cast<OpenALAudio>(source);

    alSourcei(m_BackgroundSource, AL_BUFFER, openALAudio->GetHandle());

    alSourcePlay(m_BackgroundSource);
}

void OpenALAudioAPI::SetMasterVolume(float volume)
{
    m_MasterVolume = volume;
    alListenerf(AL_GAIN, m_MasterVolume);
}

void OpenALAudioAPI::SetMusicVolume(float volume)
{
    m_MusicVolume = volume;
    alSourcef(m_BackgroundSource, AL_GAIN, m_MusicVolume);
}