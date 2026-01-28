#pragma once
#include <camera.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <render/shader.hpp>

class Renderer
{
public:
    int init();
    void renderWorld(std::vector<glm::vec3> const &cubePositions, Shader &shader, Camera &camera);
    void shutdown();

private:
    Shader shader;
};