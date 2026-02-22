#include "pch.hpp"
#include "audio/audio_api.hpp"
#include "../platform/openal/openal_audio_api.hpp"

AudioAPI::API AudioAPI::s_API = AudioAPI::API::OpenAL;

Scope<AudioAPI> AudioAPI::Create()
{
    switch (s_API)
    {
    case API::None:
        // assert
        return nullptr;
    case API::OpenAL:
        return CreateScope<OpenALAudioAPI>();
    }

    // assert
    return nullptr;
}
