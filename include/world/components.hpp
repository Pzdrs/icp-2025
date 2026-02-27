#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "asset/asset.hpp"

struct StaticMeshComponent
{
    AssetHandle meshHandle;
};

struct TransformComponent
{
    glm::vec3 translation;
    glm::vec3 scale = glm::vec3(1.0f);

    float yaw = 0.0f;  
    float pitch = 0.0f; 

    glm::mat4 GetTransform() const
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0f), translation);
        transform = glm::rotate(transform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        transform = glm::rotate(transform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::scale(transform, scale);
        return transform;
    }

    operator glm::mat4() const
    {
        return GetTransform();
    }
};

struct CircularMotionComponent
{
    float radius = 1.0f;
    float speed = 1.0f;
    float height = 0.0f;
};

struct RotationComponent
{
    float yaw = 0.0f;  
    float pitch = 0.0f; 
};