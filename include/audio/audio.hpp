#pragma once

#include "core.hpp"
#include "asset/asset.hpp"
#include <filesystem>
#include "audio/audio_api.hpp"

class Audio
{
public:
    inline static void Init()
    {
        s_AudioAPI->Init();
    }

private:
    inline static Scope<AudioAPI> s_AudioAPI = AudioAPI::Create();
};
