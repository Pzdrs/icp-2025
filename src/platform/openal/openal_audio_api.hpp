#pragma once

#include "audio/audio_api.hpp"

class OpenALAudioAPI : public AudioAPI
{
public:
    virtual void Init() override;
};