#pragma once
#include <uuid.hpp>

using AssetHandle = UUID;

enum class AssetType : uint16_t
{
    None = 0,
    Texture2D,
    Audio,
    Mesh
};

class Asset
{
public:
    virtual AssetType GetType() const = 0;

    void SetHandle(AssetHandle handle) { m_Handle = handle; }
    AssetHandle GetHandle() const { return m_Handle; }

    void SetType(AssetType type) { m_Type = type; }
    AssetType GetAssetType() const { return m_Type; }

protected:
    AssetHandle m_Handle;
    AssetType m_Type;
};
