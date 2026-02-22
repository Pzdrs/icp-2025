#include "pch.hpp"
#include "asset/asset_importer.hpp"
#include "render/texture.hpp"
#include "audio/audio.hpp"

using AssetImportFunction = std::function<Ref<Asset>(const AssetHandle handle, const std::filesystem::path &)>;
static std::map<AssetType, AssetImportFunction> s_AssetImporters = {
    {AssetType::Texture2D, TextureImporter::ImportTexture2D},
    {AssetType::Audio, AudioImporter::ImportAudio}};

Ref<Asset> AssetImporter::ImportAsset(const AssetHandle handle, const std::filesystem::path &path, AssetType type)
{
  if (s_AssetImporters.contains(type))
    return s_AssetImporters[type](handle, path);

  std::cout << "No importer found for asset type: " << static_cast<int>(type) << std::endl;
  return nullptr;
}