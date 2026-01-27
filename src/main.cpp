#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <vertex.hpp>
#include <atlas.hpp>

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
    window = glfwCreateWindow(720, 720, "Hello World", NULL, NULL);
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

    unsigned int blockAtlas = initAtlas("resources/blocks.png");

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, blockAtlas);

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}