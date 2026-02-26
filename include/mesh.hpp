#pragma once

#include "asset/asset.hpp"
#include "core.hpp"
#include <filesystem>
#include "render/vertex_array.hpp"

class Mesh : public Asset
{
public:
    virtual ~Mesh() = default;

    virtual bool operator==(const Mesh &other) const = 0;
};

class StaticMesh : public Mesh
{
public:
    StaticMesh(Scope<VertexArray> vertexArray) : m_VertexArray(std::move(vertexArray)) {}
    virtual bool operator==(const Mesh &other) const override
    {
        return false;
    };

    Scope<VertexArray> &GetVertexArray() { return m_VertexArray; }

    virtual AssetType GetType() const override { return AssetType::Mesh; }
    static AssetType GetStaticType() { return AssetType::Mesh; }

    static Ref<StaticMesh> Create(Scope<VertexArray> vertexArray);

private:
    Scope<VertexArray> m_VertexArray;
};

class MeshImporter
{
public:
    static Ref<StaticMesh> ImportMesh(const AssetHandle handle, const std::filesystem::path &path);

private:
    static Ref<StaticMesh> ImportStaticMesh(const AssetHandle handle, const std::filesystem::path &path);
};