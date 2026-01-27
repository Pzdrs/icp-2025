#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <shader.hpp>
#include "stb_image.h"
#include <glm/glm.hpp>

struct UVRect
{
    float u0, v0; // bottom-left
    float u1, v1; // top-right
};

// Get UV coordinates for a sprite at (col, row)
UVRect getSpriteUV(int col, int row, int spriteSize, int atlasWidth, int atlasHeight)
{
    float uSize = (float)spriteSize / atlasWidth;
    float vSize = (float)spriteSize / atlasHeight;

    // OpenGL UV origin is bottom-left
    float u0 = col * uSize;
    float v0 = row * vSize;
    float u1 = u0 + uSize;
    float v1 = v0 + vSize;

    return {u0, v0, u1, v1};
}

struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
};
void initAttribs()
{
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (const void *)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (const void *)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (const void *)offsetof(Vertex, texCoord));
}
unsigned int initTexture()
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("resources/atlas.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return texture;
}

int main(void)
{
    GLFWwindow *window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // macOS quirks
    // nastavit OpenGL verzi 4.1 a core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // disable deprecated functions
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1); // enable vsync

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    // macOS forcuje OpenGL Core Profile kterej defaultne nebinduje VAO - musim bindnout manualne
    // kdyz totiz neni bindnutej zadnej VAO, tak OpenGL ignoruje vsechny vertex related calls nebo co nevim
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    UVRect uv = getSpriteUV(17, 8, 16, 1024, 1024);
    std::cout << "UV: " << uv.u0 << ", " << uv.v0 << " - " << uv.u1 << ", " << uv.v1 << std::endl;

    Vertex vertices[] = {
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u0, uv.v0}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u1, uv.v0}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u0, uv.v1}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 0.0f}, {uv.u1, uv.v1}},
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 1, 3};

    unsigned int texture = initTexture();

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // index buffer (avoid vertex duplication)
    unsigned int ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    initAttribs();

    Shader shader("shaders/vertex.shader", "shaders/fragment.shader");
    shader.use();
    shader.setInt("atlas", 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}