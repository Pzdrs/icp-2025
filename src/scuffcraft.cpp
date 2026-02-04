#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <render/shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <render/vertex.hpp>
#include <atlas.hpp>
#include <camera.hpp>
#include "scuffcraft.hpp"
#include <render/vertex_buffer.hpp>
#include <render/index_buffer.hpp>
#include <render/vertex_buffer_layout.hpp>
#include <render/vertex_array.hpp>
#include <world.hpp>
#include <event/mouse_event.hpp>
#include <event/key_event.hpp>
#include "imgui_layer.hpp"
#include "input.hpp"

#define BIND_EVENT_FN(x) std::bind(&Scuffcraft::x, this, std::placeholders::_1)

Scuffcraft *Scuffcraft::s_Instance = nullptr;

void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera((float)SCR_WIDTH / (float)SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool commandWasHeld = false;

Scuffcraft::Scuffcraft()
{
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::create(WindowProps(SCR_WIDTH, SCR_HEIGHT, "Scuffcraft")));
    m_Window->setEventCallback(BIND_EVENT_FN(onEvent));

    pushLayer(new ImGuiLayer());
}

Scuffcraft::~Scuffcraft()
{
}

void Scuffcraft::pushLayer(Layer *layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Scuffcraft::pushOverlay(Layer *overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Scuffcraft::onEvent(Event &e)
{
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(onWindowsClose));
    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(onMouseMove));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(onKeyPressed));
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(onScroll));
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(onWindowResize));
    dispatcher.dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(onFramebufferResize));

    for(auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
    {
        (*--it)->OnEvent(e);
        if (e.isHandled())
            break;
    }
}

int Scuffcraft::init()
{
    renderer.init();

    Chunk::layout.push<float>(3); // position
    Chunk::layout.push<float>(3); // color
    Chunk::layout.push<float>(2); // texCoord

    loadBlockDefinitions(BLOCK_MANIFEST, blockRegistry);

    return 0;
}

void Scuffcraft::run()
{
    init();

    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    unsigned int blockAtlas = initAtlas(BLOCK_ATLAS);

    World world(blockRegistry);

    while (m_Running)
    {
        m_Window->onUpdate();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        renderer.clear();

        update(deltaTime);

        for (Layer *layer : m_LayerStack)
            layer->OnUpdate();

        render(world, shader);
    }

    shutdown();
}

void Scuffcraft::render(World &world, Shader &shader)
{
    shader.setMat4("uProjection", camera.getProjectionMatrixf(0.1f, 100.0f));
    shader.setMat4("uView", camera.getViewMatrix());
    shader.setMat4("uTransform", glm::mat4(1.0f));

    world.draw(renderer, shader);
}

void Scuffcraft::update(float deltaTime)
{
    processInput(static_cast<GLFWwindow *>(m_Window->getNativeWindow()));
}

void Scuffcraft::shutdown()
{
}

void Scuffcraft::pause()
{
    m_Paused = !m_Paused;
    m_Window->setMouseLocked(!m_Paused);
}

bool Scuffcraft::onWindowsClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

bool Scuffcraft::onWindowResize(WindowResizeEvent &e)
{
    camera.setAspectRatio((float)e.getWidth() / (float)e.getHeight());
    return true;
}

bool Scuffcraft::onFramebufferResize(FramebufferResizeEvent &e)
{
    renderer.setViewport(0, 0, e.getWidth(), e.getHeight());
    return true;
}

void processInput(GLFWwindow *window)
{

    bool commandHeld =
        Input::IsKeyPressed(GLFW_KEY_LEFT_SUPER) ||
        Input::IsKeyPressed(GLFW_KEY_RIGHT_SUPER);

    if (Input::IsKeyPressed(GLFW_KEY_W))
        camera.processKeyboard(MovementDirection::FORWARD, deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_S))
        camera.processKeyboard(MovementDirection::BACKWARD, deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_A))
        camera.processKeyboard(MovementDirection::LEFT, deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_D))
        camera.processKeyboard(MovementDirection::RIGHT, deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_SPACE))
        camera.processKeyboard(MovementDirection::UP, deltaTime);
    if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
        camera.processKeyboard(MovementDirection::DOWN, deltaTime);

    if (commandHeld && !commandWasHeld)
        camera.zoomIn();
    if (!commandHeld && commandWasHeld)
        camera.zoomOut();

    camera.tickZoom();

    commandWasHeld = commandHeld;
}

bool Scuffcraft::onMouseMove(MouseMovedEvent &e)
{
    if (m_Paused)
        return false;

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
    camera.processMouseMovement(xoffset, yoffset);
    return true;
}

bool Scuffcraft::onKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == GLFW_KEY_ESCAPE)
        pause();
    return true;
}

bool Scuffcraft::onScroll(MouseScrolledEvent &e)
{
    camera.processMouseScroll(e.getYOffset());
    return true;
}