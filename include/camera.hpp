#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdio.h>

enum class MovementDirection
{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

enum class ZoomState
{
    ZOOMED_OUT,
    ZOOMING_IN,
    ZOOMED_IN,
    ZOOMING_OUT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 5.0f;
const float SENSITIVITY = 0.1f;

const float ZOOM_SCROLL_MIN = 1.0f;
const float ZOOMED_OUT = 70.0f;
const float ZOOMED_IN = 15.0f;
const float ZOOM_SNAP_EPSILON = 0.01f;

class Camera
{
public:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;

    float movementSpeed;
    float mouseSensitivity;

    float fov;
    ZoomState zoomState = ZoomState::ZOOMED_OUT;

    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(ZOOMED_OUT)
    {
        this->position = position;
        worldUp = up;
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }
    
    Camera(
        float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch) : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), fov(ZOOMED_OUT)
    {
        this->position = glm::vec3(posX, posY, posZ);
        worldUp = glm::vec3(upX, upY, upZ);
        this->yaw = yaw;
        this->pitch = pitch;
        updateCameraVectors();
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(position, position + front, up);
    }

    void processKeyboard(MovementDirection direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;

        if (direction == MovementDirection::FORWARD)
            position += front * velocity;
        else if (direction == MovementDirection::BACKWARD)
            position -= front * velocity;
        else if (direction == MovementDirection::LEFT)
            position -= right * velocity;
        else if (direction == MovementDirection::RIGHT)
            position += right * velocity;
        else if (direction == MovementDirection::UP)
            position += up * velocity;
        else if (direction == MovementDirection::DOWN)
            position -= up * velocity;
    }

    void processMouseMovement(float xoffset, float yoffset)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // lock the pitch in <-89, 89> degrees
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        updateCameraVectors();
    }

    void processMouseScroll(float yoffset)
    {
        if (zoomState != ZoomState::ZOOMED_IN)
            return;

        fov -= (float)yoffset;
        if (fov < ZOOM_SCROLL_MIN)
            fov = ZOOM_SCROLL_MIN;
        if (fov > ZOOMED_OUT)
            fov = ZOOMED_OUT;
    }

    void zoomIn()
    {
        zoomState = ZoomState::ZOOMING_IN;
    }

    void zoomOut()
    {
        zoomState = ZoomState::ZOOMING_OUT;
    }

    void tickZoom()
    {
        if (isZoomIdle())
            return;

        const float ease = 0.12f;

        if (zoomState == ZoomState::ZOOMING_IN)
        {
            fov += (ZOOMED_IN - fov) * ease;

            if (fabs(fov - ZOOMED_IN) < ZOOM_SNAP_EPSILON)
            {
                fov = ZOOMED_IN;
                zoomState = ZoomState::ZOOMED_IN;
            }
        }
        else if (zoomState == ZoomState::ZOOMING_OUT)
        {
            fov += (ZOOMED_OUT - fov) * ease;

            if (fabs(fov - ZOOMED_OUT) < ZOOM_SNAP_EPSILON)
            {
                fov = ZOOMED_OUT;
                zoomState = ZoomState::ZOOMED_OUT;
            }
        }
    }

private:
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        this->front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(this->front, worldUp)); // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up = glm::normalize(glm::cross(right, this->front));
    }

    bool isZoomIdle()
    {
        return zoomState == ZoomState::ZOOMED_IN || zoomState == ZoomState::ZOOMED_OUT;
    }
};