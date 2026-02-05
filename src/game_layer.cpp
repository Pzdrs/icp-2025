#include "game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "atlas.hpp"
#include "render/render_command.hpp"

static const std::string BLOCK_ATLAS = "resources/blocks.png";

GameLayer::GameLayer()
    : Layer("GameLayer")
{
    initAtlas(BLOCK_ATLAS);
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
    RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    RenderCommand::Clear();
}

void GameLayer::OnImGuiRender()
{
    ImGui::ShowDemoWindow();
}

void GameLayer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
}
