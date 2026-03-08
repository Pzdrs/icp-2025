#include "pch.hpp"

#include "openal_audio_api.hpp"
#include "openal_audio.hpp"
#include <memory>

AudioAPI::SourceState TranslateALState(ALint state)
{
    switch (state)
    {
    case AL_PLAYING:
        return AudioAPI::SourceState::Playing;
    case AL_PAUSED:
        return AudioAPI::SourceState::Paused;
    case AL_INITIAL:
        return AudioAPI::SourceState::Initial;
    default:
        return AudioAPI::SourceState::Stopped;
    }
}

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

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);

    InitSources();
}

void OpenALAudioAPI::InitSources()
{
    // Music source
    alGenSources(1, &m_MusicSource);

    alSourcei(m_MusicSource, AL_SOURCE_RELATIVE, AL_TRUE);
    alSource3f(m_MusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
    alSourcef(m_MusicSource, AL_GAIN, 1.0f);

    // Source pool
    for (auto &source : m_SourcePool)
        alGenSources(1, &source.handle);
}

void OpenALAudioAPI::Shutdown()
{
    alcMakeContextCurrent(nullptr);

    if (m_Context)
        alcDestroyContext(m_Context);

    if (m_Device)
        alcCloseDevice(m_Device);
}

void OpenALAudioAPI::Update()
{
    for (auto &source : m_SourcePool)
    {
        if (!source.inUse)
            continue;

        ALint state;
        alGetSourcei(source.handle, AL_SOURCE_STATE, &state);
        if (state == AL_STOPPED)
            source.inUse = false;
    }
}

ALuint OpenALAudioAPI::AcquireSource()
{
    for (auto &source : m_SourcePool)
    {
        if (source.inUse)
            continue;

        source.inUse = true;
        return source.handle;
    }

    LOG("Out of OpenAL sources!");
    return 0;
}

void OpenALAudioAPI::PlayAt(const Ref<Audio> &source, const glm::vec3 &position)
{
    auto openALAudio = std::dynamic_pointer_cast<OpenALAudio>(source);

    ALuint sourceHandle = AcquireSource();

    alSourcei(sourceHandle, AL_SOURCE_RELATIVE, AL_FALSE);
    alSource3f(sourceHandle, AL_POSITION, position.x, position.y, position.z);

    alSourcef(sourceHandle, AL_REFERENCE_DISTANCE, 2.0f);
    alSourcef(sourceHandle, AL_ROLLOFF_FACTOR, 1.0f);
    alSourcef(sourceHandle, AL_MAX_DISTANCE, 20.0f);

    alSourcei(sourceHandle, AL_BUFFER, openALAudio->GetHandle());

    alSourcePlay(sourceHandle);
}

void OpenALAudioAPI::SetListenerPosition(glm::vec3 position)
{
    alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void OpenALAudioAPI::SetListenerOrientation(glm::vec3 forward, glm::vec3 up)
{
    float orientation[6] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
    alListenerfv(AL_ORIENTATION, orientation);
}

void OpenALAudioAPI::SetMasterVolume(float volume)
{
    m_MasterVolume = volume;
    alListenerf(AL_GAIN, m_MasterVolume);
}

///////////////////
// MUSIC
///////////////////

void OpenALAudioAPI::SetMusicVolume(float volume)
{
    m_MusicVolume = volume;
    alSourcef(m_MusicSource, AL_GAIN, m_MusicVolume);
}

void OpenALAudioAPI::PlayMusic(const Ref<Audio> &source)
{
    auto openALAudio = std::dynamic_pointer_cast<OpenALAudio>(source);

    alSourcei(m_MusicSource, AL_BUFFER, openALAudio->GetHandle());

    alSourcePlay(m_MusicSource);
}

void OpenALAudioAPI::StopMusic()
{
    alSourceStop(m_MusicSource);
}

AudioAPI::SourceState OpenALAudioAPI::GetMusicState() const
{
    ALint state;
    alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
    return TranslateALState(state);
}