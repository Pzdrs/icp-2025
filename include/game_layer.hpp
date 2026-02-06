#pragma once

#include "layer.hpp"
#include "event/application_event.hpp"
#include "event/mouse_event.hpp"
#include "render/renderer.hpp"
#include "camera_controller.hpp"
#include "world.hpp"
#include "block.hpp"
#include "render/shader.hpp"
#include <memory>

class GameLayer : public Layer
{
public:
    GameLayer();
    ~GameLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(Event &event) override;

    Camera &GetCamera() { return m_CameraController.GetCamera(); }

private:
    struct RaycastHit
    {
        bool hit = false;
        glm::ivec3 block = {0, 0, 0};
        glm::ivec3 previous = {0, 0, 0};
    };

    glm::ivec3 WorldPosToBlock(const glm::vec3 &pos) const;
    bool CollidesAt(const glm::vec3 &pos) const;
    glm::vec3 MoveAndCollide(const glm::vec3 &pos, const glm::vec3 &delta, bool &hitY);
    float FindGroundY(int worldX, int worldZ) const;
    RaycastHit Raycast(const glm::vec3 &origin, const glm::vec3 &dir, float maxDist) const;
    bool CanPlaceBlock(const glm::ivec3 &block) const;

    FreeCameraController m_CameraController;
    BlockRegistry m_BlockRegistry;
    World m_World;
    std::shared_ptr<Shader> m_Shader;

    glm::vec3 m_PlayerPos = {0.0f, 0.0f, 0.0f}; // bottom-center
    glm::vec3 m_PlayerVel = {0.0f, 0.0f, 0.0f};
    bool m_OnGround = false;

    float m_PlayerRadius = 0.3f;
    float m_PlayerHeight = 1.8f;
    float m_EyeHeight = 1.6f;
    float m_MoveSpeed = 5.0f;
    float m_JumpSpeed = 6.5f;
    float m_Gravity = -20.0f;

    bool m_LastLeftMouse = false;
    bool m_LastRightMouse = false;
};
