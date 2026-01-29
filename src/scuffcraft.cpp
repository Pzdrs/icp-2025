#include <glad/glad.h>
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

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

bool commandWasHeld = false;

int Scuffcraft::init()
{
    window.init();

    glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window.window, mouse_callback);
    glfwSetScrollCallback(window.window, scroll_callback);

    renderer.init();

    Chunk::layout.push<float>(3); // position
    Chunk::layout.push<float>(3); // color
    Chunk::layout.push<float>(2); // texCoord

    return 0;
}

void Scuffcraft::run()
{
    init();

    Shader shader("shaders/shader.vert", "shaders/shader.frag");
    unsigned int blockAtlas = initAtlas("resources/blocks.png");

    UVRect uv = getSpriteUV(17, 7, 16, 1024, 1024);

    World world;

    Vertex vertices[] = {
        // Back face (z = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}}, // 0
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},  // 1
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},   // 2
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},  // 3

        // Front face (z = 0.5)
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v1}}, // 4
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},  // 5
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},   // 6
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},  // 7

        // Left face (x = -0.5)
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},  // 8
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},   // 9
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},  // 10
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v0}}, // 11

        // Right face (x = 0.5)
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},  // 12
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},   // 13
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},  // 14
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v0}}, // 15

        // Bottom face (y = -0.5)
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}}, // 16
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},  // 17
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},   // 18
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},  // 19

        // Top face (y = 0.5)
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}}, // 20
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},  // 21
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},   // 22
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}}   // 23
    };
    std::vector<glm::vec3> cubePositions = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};
    unsigned int indices[] = {
        // Back face
        0, 1, 2,
        2, 3, 0,

        // Front face
        4, 5, 6,
        6, 7, 4,

        // Left face
        8, 9, 10,
        10, 11, 8,

        // Right face
        12, 13, 14,
        14, 15, 12,

        // Bottom face
        16, 17, 18,
        18, 19, 16,

        // Top face
        20, 21, 22,
        22, 23, 20};

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VertexBufferLayout layout;
    layout.push<float>(3); // position
    layout.push<float>(3); // color
    layout.push<float>(2); // texCoord
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices));

    va.unbind();
    vb.unbind();
    ib.unbind();

    while (!window.shouldClose())
    {
        window.pollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        renderer.clear();

        update(deltaTime);

        glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)720 / (float)720, 0.1f, 100.0f);
        shader.setMat4("uProjection", projection);
        shader.setMat4("uView", camera.getViewMatrix());

        world.draw(renderer, shader);

        int i = 0;
        for (auto &&cube : cubePositions)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cube);
            float angle = 20.0f * i;
            angle = glfwGetTime() * 25.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            shader.setMat4("uTransform", model);

            renderer.draw(va, ib, shader);
            i++;
        }

        window.swapBuffers();
    }

    shutdown();
}

void Scuffcraft::render(unsigned int blockAtlas, Shader &shader, std::vector<glm::vec3> &cubePositions)
{
}

void Scuffcraft::update(float deltaTime)
{
    processInput(window.window);
}

void Scuffcraft::shutdown()
{
    window.shutdown();
}

void processInput(GLFWwindow *window)
{

    bool commandHeld =
        glfwGetKey(window, GLFW_KEY_LEFT_SUPER) == GLFW_PRESS ||
        glfwGetKey(window, GLFW_KEY_RIGHT_SUPER) == GLFW_PRESS;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.processKeyboard(MovementDirection::DOWN, deltaTime);

    if (commandHeld && !commandWasHeld)
        camera.zoomIn();
    if (!commandHeld && commandWasHeld)
        camera.zoomOut();

    camera.tickZoom();

    commandWasHeld = commandHeld;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.processMouseScroll(static_cast<float>(yoffset));
}