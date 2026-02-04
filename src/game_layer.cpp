#include "game_layer.hpp"
#include "world.hpp"
#include "block.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "key_codes.hpp"
#include "input.hpp"
#include "scuffcraft.hpp"
#include <iostream>
#include <atlas.hpp>

float lastX = 1280 / 2.0f;
float lastY = 720 / 2.0f;
bool firstMouse = true;
bool commandWasHeld = false;

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_Camera((float)1280 / (float)720, glm::vec3(0.0f, 0.0f, 3.0f))
{
    m_Renderer.init();
    m_Shader = Shader("shaders/shader.vert", "shaders/shader.frag");
    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);
    initAtlas(BLOCK_ATLAS);
    m_World = World(m_BlockRegistry);

    Chunk::layout.push<float>(3); // position
    Chunk::layout.push<float>(3); // color
    Chunk::layout.push<float>(2); // texCoord
}

GameLayer::~GameLayer()
{
    m_Renderer.shutdown();
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
    m_Renderer.clear();

    processInput(dt);

    m_Shader.setMat4("uProjection", m_Camera.getProjectionMatrixf(0.1f, 100.0f));
    m_Shader.setMat4("uView", m_Camera.getViewMatrix());
    m_Shader.setMat4("uTransform", glm::mat4(1.0f));

    m_World.draw(m_Renderer, m_Shader);
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(Event &event)
{
    EventDispatcher dispatcher(event);
    dispatcher.dispatch<WindowResizeEvent>(std::bind(&GameLayer::onWindowResize, this, std::placeholders::_1));
    dispatcher.dispatch<MouseMovedEvent>(std::bind(&GameLayer::onMouseMove, this, std::placeholders::_1));
    dispatcher.dispatch<MouseScrolledEvent>(std::bind(&GameLayer::onScroll, this, std::placeholders::_1));
}

bool GameLayer::onMouseMove(MouseMovedEvent &e)
{
    if (firstMouse)
    {
        lastX = e.getX();
        lastY = e.getY();
        firstMouse = false;
    }

    float xoffset = e.getX() - lastX;
    float yoffset = lastY - e.getY(); // reversed since y-coordinates go from bottom to top

    lastX = e.getX();
    lastY = e.getY();
    m_Camera.processMouseMovement(xoffset, yoffset);
    return true;
}

bool GameLayer::onScroll(MouseScrolledEvent &e)
{
    m_Camera.processMouseScroll(e.getYOffset());
    return true;
}

bool GameLayer::onWindowResize(WindowResizeEvent &e)
{
    m_Camera.setAspectRatio((float)e.getWidth() / (float)e.getHeight());
    return true;
}

void GameLayer::processInput(float dt)
{
    bool commandHeld =
        Input::IsKeyPressed(Key::LeftSuper) ||
        Input::IsKeyPressed(Key::RightSuper);

    if (Input::IsKeyPressed(Key::W))
        m_Camera.processKeyboard(MovementDirection::FORWARD, dt);
    if (Input::IsKeyPressed(Key::S))
        m_Camera.processKeyboard(MovementDirection::BACKWARD, dt);
    if (Input::IsKeyPressed(Key::A))
        m_Camera.processKeyboard(MovementDirection::LEFT, dt);
    if (Input::IsKeyPressed(Key::D))
        m_Camera.processKeyboard(MovementDirection::RIGHT, dt);
    if (Input::IsKeyPressed(Key::Space))
        m_Camera.processKeyboard(MovementDirection::UP, dt);
    if (Input::IsKeyPressed(Key::LeftShift))
        m_Camera.processKeyboard(MovementDirection::DOWN, dt);

    if (commandHeld && !commandWasHeld)
        m_Camera.zoomIn();
    if (!commandHeld && commandWasHeld)
        m_Camera.zoomOut();

    m_Camera.tickZoom();
    commandWasHeld = commandHeld;
}