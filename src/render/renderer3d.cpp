#include "pch.hpp"
#include "render/renderer3d.hpp"
#include "render/shader.hpp"
#include "render/render_command.hpp"
#include "render/renderer.hpp"
#include "render/vertex.hpp"

// maybe move vertex struct here

void Renderer3D::Init()
{
}

void Renderer3D::Shutdown()
{
}

void Renderer3D::DrawMesh(const Scope<VertexArray> &vertexArray, const Ref<Material> &material, const glm::mat4 &transform)
{
    material->Bind();

    const Ref<Shader> &shader = material->GetShader();
    shader->SetInt("uTexture", 0);
    shader->SetMat4("uTransform", transform);
    shader->SetMat4("uViewProjection", Renderer::GetSceneData().ViewProjectionMatrix);

    RenderCommand::DrawIndexed(vertexArray);

    Renderer::AddDrawCall();
    Renderer::AddVertices(vertexArray->GetVertexBuffers()[0]->GetSize() / sizeof(Vertex));
    Renderer::AddIndices(vertexArray->GetIndexBuffer()->GetCount());

    s_Stats.TriangleCount += vertexArray->GetIndexBuffer()->GetCount() / 3;
    s_Stats.QuadCount += vertexArray->GetIndexBuffer()->GetCount() / 6;
}