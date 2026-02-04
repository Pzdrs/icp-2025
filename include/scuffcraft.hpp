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

    int init();
    void onEvent(Event &e);
    void pause();

    void pushLayer(Layer *layer);
    void pushOverlay(Layer *overlay);

    inline static Scuffcraft &get() { return *s_Instance; }
    inline Window &getWindow() { return *m_Window; }

    void run();
    void update(float deltaTime);
    void render(World &world, Shader &shader);
    void shutdown();

private:
    bool onWindowsClose(WindowCloseEvent &e);
    bool onWindowResize(WindowResizeEvent &e);
    bool onFramebufferResize(FramebufferResizeEvent &e);
    bool onMouseMove(MouseMovedEvent &e);
    bool onKeyPressed(KeyPressedEvent &e);
    bool onScroll(MouseScrolledEvent &e);

    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Paused = false;

    Renderer renderer;
    BlockRegistry blockRegistry;
    LayerStack m_LayerStack;

private:
    static Scuffcraft *s_Instance;
};