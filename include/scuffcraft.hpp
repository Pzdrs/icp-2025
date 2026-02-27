#pragma once
#include "window.hpp"
#include "block.hpp"
#include "world/world.hpp"
#include "event/event.hpp"
#include "event/key_event.hpp"
#include "event/application_event.hpp"
#include "layer/layer.hpp"
#include "layer/layer_stack.hpp"
#include "layer/imgui_layer.hpp"
#include "asset/asset_manager.hpp"

class Scuffcraft
{
public:
    Scuffcraft();
    virtual ~Scuffcraft();

    void Run();
    void OnEvent(Event &e);

    void PushLayer(Layer *layer);
    void PushOverlay(Layer *overlay);

    inline AssetManager &GetAssetManager() { return *m_AssetManager; }

    inline static Scuffcraft &Get() { return *s_Instance; }
    inline Window &GetWindow() { return *m_Window; }

private:
    bool OnWindowsClose(WindowCloseEvent &e);
    bool OnFramebufferResize(FramebufferResizeEvent &e);
    bool OnKeyPressed(KeyPressedEvent &e);

private:
    ImGuiLayer *m_ImGuiLayer;
    Scope<Window> m_Window;
    bool m_Running = true;
    bool m_Minimized = false;
    bool m_Fullscreen = false;

    LayerStack m_LayerStack;

    Scope<AssetManager> m_AssetManager;

private:
    static Scuffcraft *s_Instance;
};