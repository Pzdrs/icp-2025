#include "pch.hpp"
#include "block_registry.hpp"
#include "scuffcraft.hpp"

Block::ID BlockRegistry::RegisterBlock(const BlockDefinition &def)
{
    Block::ID id = static_cast<Block::ID>(m_Blocks.size());
    Ref<Texture2D> atlas = Scuffcraft::Get().GetAssetManager().GetAsset<Texture2D>(m_BlockAtlasHandle);
    m_Blocks.push_back(def);
    m_IDLookup[def.id] = id;
    m_BlockTextures[id] = Block::Texture{
        .top = SubTexture2D::CreateFromCoordinates(atlas, {def.topTex.x, def.topTex.y}, m_SpriteSize),
        .side = SubTexture2D::CreateFromCoordinates(atlas, {def.sideTex.x, def.sideTex.y}, m_SpriteSize),
        .bottom = SubTexture2D::CreateFromCoordinates(atlas, {def.bottomTex.x, def.bottomTex.y}, m_SpriteSize),
    };
    return id;
}

const BlockDefinition &BlockRegistry::Get(Block::ID id) const
{
    return m_Blocks[id];
}

const BlockDefinition &BlockRegistry::Get(const std::string &stringID) const
{
    Block::ID id = m_IDLookup.at(stringID);
    return m_Blocks[id];
}

Block::ID BlockRegistry::GetID(const std::string &stringID) const
{
    return m_IDLookup.at(stringID);
}

Ref<SubTexture2D> BlockRegistry::GetFaceTexture(Block::ID type, Block::TextureFace face) const
{
    const Block::Texture &tex = m_BlockTextures.at(type);
    switch (face)
    {
    case Block::TextureFace::SIDE:
        return tex.side;
    case Block::TextureFace::TOP:
        return tex.top;
    case Block::TextureFace::BOTTOM:
        return tex.bottom;
    default:
        throw std::runtime_error("Invalid TextureFace");
    }
}

BlockDefinition BlockRegistry::ParseBlock(const json &j)
{
    BlockDefinition b;

    b.id = j.at("id").get<std::string>();
    b.name = j.at("name").get<std::string>();
    b.isSolid = j.at("solid").get<bool>();
    b.alpha = j.at("alpha").get<float>();

    if (j.contains("textures"))
    {
        const auto &t = j.at("textures");

        b.sideTex = {
            t.at("side")[0].get<uint16_t>(),
            t.at("side")[1].get<uint16_t>()};

        b.topTex = {
            t.at("top")[0].get<uint16_t>(),
            t.at("top")[1].get<uint16_t>()};

        b.bottomTex = {
            t.at("bottom")[0].get<uint16_t>(),
            t.at("bottom")[1].get<uint16_t>()};
    }

    return b;
}

void BlockRegistry::LoadManifest(const std::string &manifestPath)
{
    std::cout << "Loading block manifest: " << manifestPath << std::endl;
    std::ifstream file(manifestPath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open block manifest: " << manifestPath << std::endl;
        return;
    }

    json j;
    file >> j;

    for (const auto &blockJson : j)
    {
        BlockDefinition blockDef = ParseBlock(blockJson);
        RegisterBlock(blockDef);
    }

    std::cout << "Loaded " << Size() << " block definitions.\n";
}