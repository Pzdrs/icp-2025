#pragma once
#define GLFW_INCLUDE_NONE
#include "window.hpp"
#include "render/renderer.hpp"
#include "render/shader.hpp"
#include "block.hpp"
#include "world.hpp"
#include "event/event.hpp"

static const std::string BLOCK_MANIFEST = "resources/blocks.json";
static const std::string BLOCK_ATLAS = "resources/blocks.png";

class Scuffcraft
{
public:
    Scuffcraft();
    virtual ~Scuffcraft();

    int init();
    void onEvent(Event &e);
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