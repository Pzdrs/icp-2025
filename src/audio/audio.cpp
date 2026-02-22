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
        std::cout << "Failed to load audio file: " << path << std::endl;
        return nullptr;
    }

    uint32_t dataSize = static_cast<uint32_t>(mp3.totalPCMFrameCount * mp3.channels * sizeof(int16_t));
    std::vector<int16_t> audioData(mp3.totalPCMFrameCount * mp3.channels);
    drmp3_read_pcm_frames_s16(&mp3, mp3.totalPCMFrameCount, audioData.data());
    drmp3_uninit(&mp3);

    Ref<Audio> audioSource = Audio::Create(audioData.data(), dataSize, mp3.sampleRate, mp3.channels);
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
