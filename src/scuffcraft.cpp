#include "pch.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "scuffcraft.hpp"
#include <event/mouse_event.hpp>
#include <event/key_event.hpp>
#include "imgui_layer.hpp"
#include "game_layer.hpp"
#include "debug_layer.hpp"
#include "input.hpp"
#include "key_codes.hpp"

Scuffcraft *Scuffcraft::s_Instance = nullptr;

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Scuffcraft::Scuffcraft()
{
    s_Instance = this;
    m_Window = Scope<Window>(Window::Create(WindowProps(SCR_WIDTH, SCR_HEIGHT, "Scuffcraft")));
    m_Window->SetEventCallback(BIND_EVENT_FN(Scuffcraft::OnEvent));
    m_Window->SetVSync(true);
    m_Window->SetMouseLocked(true);

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

bool Scuffcraft::OnWindowsClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

bool Scuffcraft::OnFramebufferResize(FramebufferResizeEvent &e)
{
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    return true;
}