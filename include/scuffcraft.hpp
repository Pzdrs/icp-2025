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

static const std::string BLOCK_MANIFEST = "resources/blocks.json";
static const std::string BLOCK_ATLAS = "resources/blocks.png";

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
    bool OnWindowResize(WindowResizeEvent &e);
    bool OnFramebufferResize(FramebufferResizeEvent &e);
    bool OnMouseMove(MouseMovedEvent &e);
    bool OnKeyPressed(KeyPressedEvent &e);
    bool OnScroll(MouseScrolledEvent &e);

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Paused = false;

    Renderer m_Renderer;
    BlockRegistry m_BlockRegistry;
    LayerStack m_LayerStack;

private:
    static Scuffcraft *s_Instance;
};