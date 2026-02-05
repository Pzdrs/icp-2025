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
#include <world.hpp>
#include <event/mouse_event.hpp>
#include <event/key_event.hpp>
#include "imgui_layer.hpp"
#include "game_layer.hpp"
#include "input.hpp"
#include "render/render_command.hpp"
#include "key_codes.hpp"

#define BIND_EVENT_FN(x) std::bind(&Scuffcraft::x, this, std::placeholders::_1)

Scuffcraft *Scuffcraft::s_Instance = nullptr;

void processInput();

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera((float)SCR_WIDTH / (float)SCR_HEIGHT, glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool commandWasHeld = false;

Scuffcraft::Scuffcraft() : m_Camera(70.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f)
{
    s_Instance = this;
    m_Window = std::unique_ptr<Window>(Window::create(WindowProps(SCR_WIDTH, SCR_HEIGHT, "Scuffcraft")));
    m_Window->setEventCallback(BIND_EVENT_FN(OnEvent));

    Renderer::Init();

    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);

    m_ImGuiLayer = new ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    auto gameLayer = new GameLayer();
    PushLayer(gameLayer);

    m_Camera.SetPosition(glm::vec3(-1.0f, 0.0f, 3.0f));
    m_Camera.SetPitchYaw(0.0f, -90.0f);
}

Scuffcraft::~Scuffcraft()
{
    Renderer::Shutdown();
}

void Scuffcraft::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);

    dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowsClose));
    dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

    dispatcher.dispatch<MouseMovedEvent>(BIND_EVENT_FN(OnMouseMove));
    dispatcher.dispatch<KeyPressedEvent>(BIND_EVENT_FN(OnKeyPressed));
    dispatcher.dispatch<MouseScrolledEvent>(BIND_EVENT_FN(OnScroll));
    dispatcher.dispatch<FramebufferResizeEvent>(BIND_EVENT_FN(OnFramebufferResize));
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
    {
        (*--it)->OnEvent(e);
        if (e.isHandled())
            break;
    }
}

void Scuffcraft::Run()
{
    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    World world(m_BlockRegistry);

    while (m_Running)
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        for (Layer *layer : m_LayerStack)
            layer->OnUpdate(deltaTime);

        // soon to be gone

        Renderer::BeginScene();
        shader.setMat4("uProjection", camera.getProjectionMatrixf(0.1f, 100.0f));
        shader.setMat4("uView", camera.getViewMatrix());
        shader.setMat4("uTransform", glm::mat4(1.0f));
        world.draw(shader);
        Renderer::EndScene();

        // soon to be gone

        m_ImGuiLayer->Begin();
        for (Layer *layer : m_LayerStack)
            layer->OnImGuiRender();
        m_ImGuiLayer->End();

        m_Window->onUpdate();
    }
}

void Scuffcraft::PushLayer(Layer *layer)
{
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}

void Scuffcraft::PushOverlay(Layer *overlay)
{
    m_LayerStack.PushOverlay(overlay);
    overlay->OnAttach();
}

void Scuffcraft::Pause()
{
    m_Paused = !m_Paused;
    m_Window->setMouseLocked(!m_Paused);
}

bool Scuffcraft::OnWindowsClose(WindowCloseEvent &e)
{
    m_Running = false;
    return true;
}

bool Scuffcraft::OnWindowResize(WindowResizeEvent &e)
{
    camera.setAspectRatio((float)e.getWidth() / (float)e.getHeight());
    return true;
}

bool Scuffcraft::OnFramebufferResize(FramebufferResizeEvent &e)
{
    Renderer::OnWindowResize(e.getWidth(), e.getHeight());
    return true;
}

void processInput()
{

    bool commandHeld =
        Input::IsKeyPressed(Key::LeftSuper) ||
        Input::IsKeyPressed(Key::RightSuper);

    if (Input::IsKeyPressed(Key::W))
        camera.processKeyboard(MovementDirection::FORWARD, deltaTime);
    if (Input::IsKeyPressed(Key::S))
        camera.processKeyboard(MovementDirection::BACKWARD, deltaTime);
    if (Input::IsKeyPressed(Key::A))
        camera.processKeyboard(MovementDirection::LEFT, deltaTime);
    if (Input::IsKeyPressed(Key::D))
        camera.processKeyboard(MovementDirection::RIGHT, deltaTime);
    if (Input::IsKeyPressed(Key::Space))
        camera.processKeyboard(MovementDirection::UP, deltaTime);
    if (Input::IsKeyPressed(Key::LeftShift))
        camera.processKeyboard(MovementDirection::DOWN, deltaTime);

    if (commandHeld && !commandWasHeld)
        camera.zoomIn();
    if (!commandHeld && commandWasHeld)
        camera.zoomOut();

    camera.tickZoom();

    commandWasHeld = commandHeld;
}

bool Scuffcraft::OnMouseMove(MouseMovedEvent &e)
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

bool Scuffcraft::OnKeyPressed(KeyPressedEvent &e)
{
    if (e.getKeyCode() == Key::Escape)
        Pause();
    return true;
}

bool Scuffcraft::OnScroll(MouseScrolledEvent &e)
{
    camera.processMouseScroll(e.getYOffset());
    return true;
}