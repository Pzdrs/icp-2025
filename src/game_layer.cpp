#include "game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>

GameLayer::GameLayer()
    : Layer("GameLayer")
{
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
}

void GameLayer::OnImGuiRender()
{
    ImGui::ShowDemoWindow();
}

void GameLayer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
}
