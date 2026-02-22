#pragma once
#include "asset/asset.hpp"
#include <filesystem>
#include "core.hpp"

class AudioSource : public Asset
{
    virtual ~AudioSource() = default;

    virtual void SetData(const void *data, uint32_t size) = 0;

    static Ref<AudioSource> Create(const std::filesystem::path &path);
};

class AudioImporter
{
public:
    static Ref<AudioSource> ImportAudioSource(const AssetHandle handle, const std::filesystem::path &path);
};