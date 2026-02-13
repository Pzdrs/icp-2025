#include "pch.hpp"
#include "asset/asset_manager.hpp"
#include "asset/asset_importer.hpp"

AssetHandle AssetManager::LoadAsset(const std::filesystem::path &path, AssetType type)
{
    if (m_PathToHandle.contains(path))
        return m_PathToHandle[path];

    AssetHandle handle; // generate new handle

    Ref<Asset> asset = AssetImporter::ImportAsset(handle, path, type);
    if (!asset)
    {
        std::cout << "Failed to import asset: " << path << std::endl;
        return 0;
    }

    m_Assets[handle] = asset;
    m_PathToHandle[path] = handle;
    return handle;
}