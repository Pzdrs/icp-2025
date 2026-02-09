#pragma once

#include "core.hpp"
#include "render/texture.hpp"
#include <block.hpp>

using json = nlohmann::json;

// Describes a block as loaded in from a manifest.
struct BlockDefinition
{
    std::string id, name;

    bool isSolid;
    float alpha;

    glm::ivec2 topTex, sideTex, bottomTex;
};

class BlockRegistry
{
public:
    BlockRegistry(const Ref<Texture2D> &blockAtlas)
        : m_BlockAtlas(blockAtlas)
    {
    }

    Block::ID RegisterBlock(const BlockDefinition &def);

    const BlockDefinition &Get(Block::ID id) const;
    const BlockDefinition &Get(const std::string &stringID) const;

    Block::ID GetID(const std::string &stringID) const;

    Ref<SubTexture2D> GetFaceTexture(Block::ID type, Block::TextureFace face) const;

    size_t Size() const { return m_Blocks.size(); }

    void LoadManifest(const std::string &manifestPath);

private:
    BlockDefinition ParseBlock(const json &j);

private:
    Ref<Texture2D> m_BlockAtlas;

    std::vector<BlockDefinition> m_Blocks;
    std::unordered_map<std::string, Block::ID> m_IDLookup;
    std::unordered_map<Block::ID, Block::Texture> m_BlockTextures;
};
