#pragma once

#include "core.hpp"
#include "asset/asset.hpp"
#include <filesystem>
#include "audio/audio_api.hpp"
#include "audio/audio.hpp"

class AudioEngine
{
public:
    inline static void Init()
    {
        s_AudioAPI->Init();
    }

    inline static void Shutdown()
    {
        s_AudioAPI->Shutdown();
    }

    inline static void PlayBackground(const Ref<Audio> &source)
    {
        s_AudioAPI->PlayBackground(source);
    }

    inline static float GetMasterVolume() { return s_AudioAPI->GetMasterVolume(); }
    inline static void SetMasterVolume(float volume) { s_AudioAPI->SetMasterVolume(volume); }

    inline static float GetMusicVolume() { return s_AudioAPI->GetMusicVolume(); }
    inline static void SetMusicVolume(float volume) { s_AudioAPI->SetMusicVolume(volume); }

    inline static AudioAPI::API
    CurrentAPI()
    {
        return AudioAPI::CurrentAPI();
    }

private:
    inline static Scope<AudioAPI> s_AudioAPI = AudioAPI::Create();
};
