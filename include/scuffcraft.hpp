#pragma once
#include "window.hpp"
#include "render/renderer.hpp"
#include "render/shader.hpp"
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
    bool onWindowsClose(WindowCloseEvent &e);
    // bool onWindowResize(WindowResizeEvent &e);
    bool onFramebufferResize(FramebufferResizeEvent &e);
    bool onKeyPressed(KeyPressedEvent &e);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Paused = false;

    LayerStack m_LayerStack;
    ImGuiLayer *m_ImGuiLayer;

private:
    static Scuffcraft *s_Instance;
};