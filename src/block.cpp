#include "block.hpp"
#include <fstream>
#include <iostream>

bool isSideFace(Face face)
{
    return face == Face::FACE_BACK || face == Face::FACE_FRONT || face == Face::FACE_LEFT || face == Face::FACE_RIGHT;
}

BlockTexture getBlockTexture(const BlockID type, const BlockRegistry &blockRegistry)
{
    return blockRegistry.get(type).texture;
}

BlockID BlockRegistry::registerBlock(const BlockDefinition &def)
{
    BlockID id = static_cast<BlockID>(blocks.size());
    blocks.push_back(def);
    idLookup[def.id] = id;
    return id;
}

const BlockDefinition &BlockRegistry::get(BlockID id) const
{
    return blocks[id];
}

const BlockDefinition &BlockRegistry::get(const std::string &stringID) const
{
    BlockID id = idLookup.at(stringID);
    return blocks[id];
}

BlockID BlockRegistry::getID(const std::string &stringID) const
{
    return idLookup.at(stringID);
}

BlockDefinition parseBlock(const json &j)
{
    BlockDefinition b;

    b.id = j.at("id").get<std::string>();
    b.name = j.at("name").get<std::string>();
    b.isSolid = j.at("solid").get<bool>();
    b.alpha = j.at("alpha").get<float>();

    if (j.contains("textures"))
    {
        const auto &t = j.at("textures");

        b.texture.side = {
            t.at("side")[0].get<uint16_t>(),
            t.at("side")[1].get<uint16_t>()};

        b.texture.top = {
            t.at("top")[0].get<uint16_t>(),
            t.at("top")[1].get<uint16_t>()};

        b.texture.bottom = {
            t.at("bottom")[0].get<uint16_t>(),
            t.at("bottom")[1].get<uint16_t>()};
    }

    return b;
}

void loadBlockDefinitions(const std::string &manifestPath, BlockRegistry &blockRegistry)
{
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
        BlockDefinition blockDef = parseBlock(blockJson);
        blockRegistry.registerBlock(blockDef);
    }

    std::cout << "Loaded " << blockRegistry.size() << " block definitions.\n";
}