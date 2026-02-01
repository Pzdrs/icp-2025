#pragma once
#define GLFW_INCLUDE_NONE
#include "window.hpp"
#include "render/renderer.hpp"
#include "render/shader.hpp"
#include "block.hpp"
#include "world.hpp"

static const std::string BLOCK_MANIFEST = "resources/blocks.json";
static const std::string BLOCK_ATLAS = "resources/blocks.png";

class Scuffcraft
{
public:
    int init();
    void run();
    void update(float deltaTime);
    void render(World &world, Shader &shader);
    void shutdown();

private:
    Window window;
    Renderer renderer;

    BlockRegistry blockRegistry;
};