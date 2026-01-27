#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <assert.h>
#include <shader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vertex.hpp>
#include <atlas.hpp>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);

const unsigned int SCR_WIDTH = 720;
const unsigned int SCR_HEIGHT = 720;

int main(void)
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // enable vsync

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    Shader shader("shaders/vertex.shader", "shaders/fragment.shader");

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

    shader.use();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, blockAtlas);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        shader.setMat4("model", model);
        shader.setMat4("view", view);
        // possible optimization: only set projection matrix once outside of the loop if it never changes
        shader.setMat4("projection", projection);

        // glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, nullptr);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}