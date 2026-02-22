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

    inline static void Play(const Ref<Audio> &source)
    {
        s_AudioAPI->Play(source);
    }

    inline static AudioAPI::API CurrentAPI() { return AudioAPI::CurrentAPI(); }

private:
    inline static Scope<AudioAPI> s_AudioAPI = AudioAPI::Create();
};
