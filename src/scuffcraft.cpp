#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <render/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/vertex.hpp>
#include <atlas.hpp>
#include <camera.hpp>
#include "scuffcraft.hpp"
#include <world.hpp>
#include <event/mouse_event.hpp>
#include <event/key_event.hpp>
#include "imgui_layer.hpp"
#include "game_layer.hpp"
#include "debug_layer.hpp"
#include "input.hpp"
#include "render/render_command.hpp"
#include "key_codes.hpp"
#include <camera_controller.hpp>
#include <core.hpp>

Scuffcraft *Scuffcraft::s_Instance = nullptr;

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Scuffcraft::Scuffcraft()
{
    s_Instance = this;
    m_Window = Scope<Window>(Window::Create(WindowProps(SCR_WIDTH, SCR_HEIGHT, "Scuffcraft")));
    m_Window->SetEventCallback(BIND_EVENT_FN(Scuffcraft::OnEvent));

    Renderer::Init();

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    auto gameLayer = new GameLayer();
    PushLayer(gameLayer);

    auto debugLayer = new DebugLayer(gameLayer->GetCamera());
    PushOverlay(debugLayer);
}

Scuffcraft::~Scuffcraft()
{
    Renderer::Shutdown();
}

void Scuffcraft::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(Scuffcraft::OnFramebufferResize));
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Scuffcraft::OnWindowsClose));

    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(Scuffcraft::OnMouseMoved));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(Scuffcraft::OnKeyPressed));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.isHandled())
            break;
    }
}

void Scuffcraft::Run()
{
    while (m_Running)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        m_DeltaTime = currentFrame - m_LastFrameTime;
        m_LastFrameTime = currentFrame;

        for (Layer *layer : m_LayerStack)
            layer->OnUpdate(m_DeltaTime);

        m_ImGuiLayer->Begin();
        for (Layer *layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->OnUpdate();
    }
}

void Scuffcraft::PushLayer(Layer *layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Scuffcraft::PushOverlay(Layer *overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Scuffcraft::Pause()
{
    m_Paused = !m_Paused;
    m_Window->SetMouseLocked(!m_Paused);
}

bool Scuffcraft::OnWindowsClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

// so the camera doesn't process mouse movement when the game is paused
bool Scuffcraft::OnMouseMoved(MouseMovedEvent &e)
{
    if (m_Paused)
        return true;
    return false;
}

bool Scuffcraft::OnFramebufferResize(FramebufferResizeEvent &e)
{
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    return true;
}

bool Scuffcraft::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == Key::Escape)
    {
        Pause();
        return true;
    }

    return false;
}