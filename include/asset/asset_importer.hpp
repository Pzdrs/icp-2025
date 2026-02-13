#pragma once
#include "core.hpp"
#include "asset/asset.hpp"

class AssetImporter
{
public:
    static Ref<Asset> ImportAsset(const AssetHandle handle, const std::filesystem::path &path, AssetType type);
};