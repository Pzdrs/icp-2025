#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "render/renderer.hpp"
#include <stdio.h>

int Renderer::init()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    glEnable(GL_DEPTH_TEST);
    return 0;
}

void Renderer::shutdown()
{
    // Cleanup code for the renderer would go here
}

void Renderer::renderWorld(const std::vector<glm::vec3> &cubePositions, Shader &shader, Camera &camera)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)720 / (float)720, 0.1f, 100.0f);
    shader.setMat4("projection", projection);
    shader.setMat4("view", camera.getViewMatrix());

    int i = 0;
    for (auto &&cube : cubePositions)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cube);
        float angle = 20.0f * i;
        angle = glfwGetTime() * 25.0f;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);
        i++;
    }
}