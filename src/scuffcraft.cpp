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

    return 0;
}

void Scuffcraft::run()
{
    init();

    Shader shader("shaders/shader.vert", "shaders/shader.frag");

    UVRect uv = getSpriteUV(17, 8, 16, 1024, 1024);

    // Vertex vertices[] = {
    //     {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u0, uv.v0}},
    //     {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u1, uv.v0}},
    //     {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u0, uv.v1}},
    //     {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u1, uv.v1}},
    // };
    Vertex vertices[] = {
        // back face
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},

        // front face
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},

        // left face
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},

        // right face
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},

        // bottom face
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{-0.5f, -0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{-0.5f, -0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},

        // top face
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
        {{0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u1, uv.v1}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u1, uv.v0}},
        {{-0.5f, 0.5f, 0.5f}, {0, 0, 0}, {uv.u0, uv.v0}},
        {{-0.5f, 0.5f, -0.5f}, {0, 0, 0}, {uv.u0, uv.v1}},
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
        // back
        0, 1, 2,
        2, 1, 3,

        // front
        4, 5, 6,
        6, 5, 7,

        // left
        8, 9, 10,
        10, 9, 11,

        // right
        12, 13, 14,
        14, 13, 15,

        // bottom
        16, 17, 18,
        18, 17, 19,

        // top
        20, 21, 22,
        22, 21, 23};

    // macOS forcuje OpenGL Core Profile kterej defaultne nebinduje VAO - musim bindnout manualne
    // kdyz totiz neni bindnutej zadnej VAO, tak OpenGL ignoruje vsechny vertex related calls nebo co nevim
    unsigned int VBO, VAO, IBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &IBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    initAttribs();

    // index buffer (avoid vertex duplication)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    unsigned int blockAtlas = initAtlas("resources/blocks.png");
    glBindTexture(GL_TEXTURE_2D, blockAtlas);


    while (!window.shouldClose())
    {
        window.pollEvents();
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window.window);

        render(blockAtlas, shader, cubePositions);

        window.swapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &IBO);

    shutdown();
}

void Scuffcraft::render(unsigned int blockAtlas, Shader &shader, std::vector<glm::vec3> &cubePositions)
{
    renderer.renderWorld(cubePositions, shader, camera);
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