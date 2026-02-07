#pragma once
#include "window.hpp"
#include "block.hpp"
#include "world.hpp"
#include "event/event.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "layer.hpp"
#include "layer_stack.hpp"
#include "imgui_layer.hpp"


class Scuffcraft
{
public:
    Scuffcraft();
    virtual ~Scuffcraft();

    void OnEvent(Event &e);
    void Pause();

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);

    inline static Scuffcraft &Get() { return *s_Instance; }
    inline Window &GetWindow() { return *m_Window; }

    void Run();

private:
    bool OnWindowsClose(WindowCloseEvent &e);
    bool OnFramebufferResize(FramebufferResizeEvent &e);
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnMouseMoved(MouseMovedEvent &e);

private:
    ImGuiLayer *m_ImGuiLayer;
    Scope<Window> m_Window;
    bool m_Running = true;
    bool m_Paused = false;

    float m_DeltaTime = 0.0f;
    float m_LastFrameTime = 0.0f;

    LayerStack m_LayerStack;

private:
    static Scuffcraft *s_Instance;
};