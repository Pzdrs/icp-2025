#include "pch.hpp"
#include "audio/audio_engine.hpp"
#include "audio/audio.hpp"
#include "dr_mp3.hpp"
#include "../platform/openal/openal_audio.hpp"

Ref<Audio> LoadAudioSourceMP3(const AssetHandle handle, const std::filesystem::path &path)
{
    drmp3 mp3;

    if (!drmp3_init_file(&mp3, path.string().c_str(), nullptr))
    {
        LOG("Failed to load audio file: %s", path.string().c_str());
        return nullptr;
    }

    std::vector<int16_t> audioData;

    const uint64_t chunkFrames = 4096;
    std::vector<int16_t> tempBuffer(chunkFrames * mp3.channels);

    uint64_t framesRead;

    while ((framesRead = drmp3_read_pcm_frames_s16(
                &mp3,
                chunkFrames,
                tempBuffer.data())) > 0)
    {
        size_t samplesRead = framesRead * mp3.channels;
        audioData.insert(audioData.end(),
                         tempBuffer.begin(),
                         tempBuffer.begin() + samplesRead);
    }

    uint32_t sampleRate = mp3.sampleRate;
    uint32_t channels = mp3.channels;
    drmp3_uninit(&mp3);

    if (audioData.empty())
        return nullptr;

    size_t dataSize = audioData.size() * sizeof(int16_t);

    Ref<Audio> audioSource = Audio::Create(audioData.data(), dataSize, sampleRate, channels);

    if (audioSource)
    {
        audioSource->SetType(AssetType::Audio);
        audioSource->SetHandle(handle);
    }

    return audioSource;
}

Ref<Audio> Audio::Create(const void *data, uint32_t size, uint32_t sampleRate, uint32_t channels)
{
    switch (AudioEngine::CurrentAPI())
    {
    case AudioAPI::API::None:
        // assert
        return nullptr;
    case AudioAPI::API::OpenAL:
        return CreateRef<OpenALAudio>(data, size, sampleRate, channels);
    }

    // assert
    return nullptr;
}

Ref<Audio> AudioImporter::ImportAudio(const AssetHandle handle, const std::filesystem::path &path)
{
    return LoadAudioSourceMP3(handle, path);
}
