#include "pch.hpp"
#include "render/renderer3d.hpp"
#include "render/shader.hpp"
#include "render/render_command.hpp"
#include "render/renderer.hpp"

void Renderer3D::Init()
{
}

void Renderer3D::Shutdown()
{
}

void Renderer3D::DrawMesh(const Ref<Shader> &shader, const Ref<VertexArray> &vertexArray, const glm::mat4 &transform)
{
    shader->Bind();
    shader->SetInt("uTexture", 0);
    shader->SetMat4("uTransform", transform);
    shader->SetMat4("uViewProjection", Renderer::GetSceneData().ViewProjectionMatrix);

    RenderCommand::DrawIndexed(vertexArray);

    Renderer::AddDrawCall();
    Renderer::AddTriangles(vertexArray->GetIndexBuffer()->GetCount() / 3);
}