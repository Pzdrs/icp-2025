#include "pch.hpp"
#include "mesh.hpp"
#include <tiny_obj_loader.h>
#include "render/vertex.hpp"

Ref<StaticMesh> StaticMesh::Create(Scope<VertexArray> vertexArray)
{
    return CreateRef<StaticMesh>(std::move(vertexArray));
}

Ref<StaticMesh> MeshImporter::ImportMesh(const AssetHandle handle, const std::filesystem::path &path)
{
    return ImportStaticMesh(handle, path);
}

Ref<StaticMesh> MeshImporter::ImportStaticMesh(const AssetHandle handle, const std::filesystem::path &path)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    std::string warn;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.string().c_str());

    if (!err.empty())
        std::cerr << err << std::endl;

    if (!ret)
        return nullptr;

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex vertex{};

            vertex.position = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2],
            };

            if (index.texcoord_index >= 0)
            {
                vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1],
                };
            }

            vertex.color = {0.0f, 0.0f, 0.0f, 1.0f};

            vertices.push_back(vertex);
            indices.push_back(static_cast<uint32_t>(indices.size()));
        }
    }

    auto vb = VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
    auto ib = IndexBuffer::Create(indices.data(), indices.size() * sizeof(uint32_t));
    auto vao = VertexArray::Create();

    vb->SetLayout({
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"},
        {ShaderDataType::Float2, "a_TexCoord"},
    });
    vao->AddVertexBuffer(std::move(vb));
    vao->SetIndexBuffer(std::move(ib));

    Ref<StaticMesh> mesh = StaticMesh::Create(std::move(vao));
    return mesh;
}