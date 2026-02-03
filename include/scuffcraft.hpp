#pragma once
#define GLFW_INCLUDE_NONE
#include "window.hpp"
#include "render/renderer.hpp"
#include "render/shader.hpp"
#include "block.hpp"
#include "world.hpp"
#include "event/event.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"

static const std::string BLOCK_MANIFEST = "resources/blocks.json";
static const std::string BLOCK_ATLAS = "resources/blocks.png";

class Scuffcraft
{
public:
    Scuffcraft();
    virtual ~Scuffcraft();

    int init();
    void onEvent(Event &e);
    bool onWindowsClose(WindowCloseEvent &e);
    bool onWindowResize(WindowResizeEvent &e);
    bool onFramebufferResize(FramebufferResizeEvent &e);
    bool onMouseMove(MouseMovedEvent &e);
    bool onKeyPressed(KeyPressedEvent &e);
    bool onScroll(MouseScrolledEvent &e);
    void pause();


    void run();
    void update(float deltaTime);
    void render(World &world, Shader &shader);
    void shutdown();

private:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;
    bool m_Paused = false;

    Renderer renderer;
    BlockRegistry blockRegistry;
};