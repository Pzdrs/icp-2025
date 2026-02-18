#include "pch.hpp"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "scuffcraft.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "layer/imgui_layer.hpp"
#include "layer/game_layer.hpp"
#include "layer/debug_layer.hpp"
#include "input.hpp"
#include "key_codes.hpp"
#include "screenshot.hpp"
#include "job_system.hpp"

Scuffcraft *Scuffcraft::s_Instance = nullptr;

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Scuffcraft::Scuffcraft()
    : m_AssetManager(CreateScope<AssetManager>())
{
    s_Instance = this;
    m_Window = Scope<Window>(Window::Create(WindowProps(SCR_WIDTH, SCR_HEIGHT, "Scuffcraft")));
    m_Window->SetEventCallback(BIND_EVENT_FN(Scuffcraft::OnEvent));
    m_Window->SetVSync(true);
    m_Window->SetMouseLocked(true);

    Renderer::Init();
    JobSystem::Init(1);

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    auto gameLayer = new GameLayer();
    PushLayer(gameLayer);

    auto debugLayer = new DebugLayer(gameLayer->GetCamera());
    PushOverlay(debugLayer);
}

Scuffcraft::~Scuffcraft()
{
    JobSystem::Shutdown();
    Renderer::Shutdown();
}

void Scuffcraft::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(Scuffcraft::OnFramebufferResize));
    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Scuffcraft::OnWindowsClose));
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

bool Scuffcraft::OnWindowsClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

bool Scuffcraft::OnFramebufferResize(FramebufferResizeEvent &e)
{
    if (e.getWidth() == 0 || e.getHeight() == 0)
    {
        m_Minimized = true;
        return false;
    }

    m_Minimized = false;
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    return true;
}

bool Scuffcraft::OnKeyPressed(KeyPressedEvent &e)
{

    return false;
}