#pragma once
#include "asset/asset.hpp"
#include <filesystem>
#include "core.hpp"

class Audio : public Asset
{
public:
    virtual ~Audio() = default;

    static AssetType GetStaticType() { return AssetType::Audio; }
    virtual AssetType GetType() const override { return GetStaticType(); }

    static Ref<Audio> Create(const void *data, uint32_t size, uint32_t sampleRate = 44100, uint32_t channels = 2);
};

class AudioImporter
{
public:
    static Ref<Audio> ImportAudio(const AssetHandle handle, const std::filesystem::path &path);
};
