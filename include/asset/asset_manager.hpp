#pragma once
#include "core.hpp"
#include "asset/asset.hpp"
#include <map>

using AssetMap = std::map<AssetHandle, Ref<Asset>>;

class AssetManager
{
public:
    AssetHandle LoadAsset(const std::filesystem::path &path, AssetType type);

    template <typename T>
    Ref<T> GetAsset(const AssetHandle &handle)
    {
        if (!m_Assets.contains(handle))
            return nullptr;

        return std::dynamic_pointer_cast<T>(m_Assets[handle]);
    }

private:
    AssetMap m_Assets;
    std::unordered_map<std::filesystem::path, AssetHandle> m_PathToHandle;
};