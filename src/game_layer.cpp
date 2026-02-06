#include "game_layer.hpp"
#include "event/mouse_event.hpp"
#include "event/key_event.hpp"
#include "scuffcraft.hpp"
#include <imgui.h>
#include "chunk.hpp"
#include "atlas.hpp"
#include "render/render_command.hpp"
#include "input.hpp"
#include "key_codes.hpp"
#include "mouse_codes.hpp"
#include <cmath>
#include <algorithm>

static const std::string BLOCK_ATLAS = "resources/blocks.png";
static const std::string BLOCK_MANIFEST = "resources/blocks.json";

GameLayer::GameLayer()
    : Layer("GameLayer"),
      m_CameraController((float)Scuffcraft::Get().GetWindow().GetWidth() / (float)Scuffcraft::Get().GetWindow().GetHeight()),
      m_Shader(std::make_shared<Shader>("shaders/shader.vert", "shaders/shader.frag"))
{
    initAtlas(BLOCK_ATLAS);
    loadBlockDefinitions(BLOCK_MANIFEST, m_BlockRegistry);
    m_World.Generate(m_BlockRegistry);

    m_CameraController.SetMovementEnabled(false);

    // Spawn the player roughly in the middle of the world
    const int spawnX = Chunk::SIZE_XZ / 2;
    const int spawnZ = Chunk::SIZE_XZ / 2;
    const float groundY = FindGroundY(spawnX, spawnZ);
    m_PlayerPos = glm::vec3((float)spawnX + 0.5f, groundY, (float)spawnZ + 0.5f);

    m_CameraController.SetPosition(m_PlayerPos + glm::vec3(0.0f, m_EyeHeight, 0.0f));
    m_CameraController.SetPitchYaw(0.0f, -90.0f);
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnAttach()
{
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(float dt)
{
    m_CameraController.OnUpdate(dt);

    // Movement input (XZ plane)
    glm::vec3 forward = m_CameraController.GetCamera().GetForward();
    forward.y = 0.0f;
    if (glm::length(forward) > 0.0001f)
        forward = glm::normalize(forward);

    glm::vec3 right = m_CameraController.GetCamera().GetRight();
    right.y = 0.0f;
    if (glm::length(right) > 0.0001f)
        right = glm::normalize(right);

    glm::vec3 moveDir(0.0f);
    if (Input::IsKeyPressed(Key::W))
        moveDir += forward;
    if (Input::IsKeyPressed(Key::S))
        moveDir -= forward;
    if (Input::IsKeyPressed(Key::A))
        moveDir -= right;
    if (Input::IsKeyPressed(Key::D))
        moveDir += right;

    if (glm::length(moveDir) > 0.0001f)
        moveDir = glm::normalize(moveDir);

    // Jump
    if (m_OnGround && Input::IsKeyPressed(Key::Space))
    {
        m_PlayerVel.y = m_JumpSpeed;
        m_OnGround = false;
    }

    // Gravity
    m_PlayerVel.y += m_Gravity * dt;

    glm::vec3 pos = m_PlayerPos;

    // Horizontal movement
    glm::vec3 horizontal = moveDir * m_MoveSpeed * dt;
    bool hitY = false;
    pos = MoveAndCollide(pos, glm::vec3(horizontal.x, 0.0f, 0.0f), hitY);
    pos = MoveAndCollide(pos, glm::vec3(0.0f, 0.0f, horizontal.z), hitY);

    // Vertical movement
    pos = MoveAndCollide(pos, glm::vec3(0.0f, m_PlayerVel.y * dt, 0.0f), hitY);
    if (hitY)
        m_PlayerVel.y = 0.0f;

    m_PlayerPos = pos;

    // Update grounded state by probing slightly below the player
    glm::vec3 probePos = m_PlayerPos + glm::vec3(0.0f, -0.02f, 0.0f);
    m_OnGround = CollidesAt(probePos);

    m_CameraController.SetPosition(m_PlayerPos + glm::vec3(0.0f, m_EyeHeight, 0.0f));

    // Block breaking/placing
    const bool left = Input::IsMouseButtonPressed(Mouse::ButtonLeft);
    const bool rightBtn = Input::IsMouseButtonPressed(Mouse::ButtonRight);

    if (left && !m_LastLeftMouse)
    {
        RaycastHit hit = Raycast(m_CameraController.GetCamera().GetPosition(),
                                 m_CameraController.GetCamera().GetForward(),
                                 6.0f);
        if (hit.hit)
        {
            m_World.SetBlock(hit.block.x, hit.block.y, hit.block.z, m_BlockRegistry.getID("air"), m_BlockRegistry);
        }
    }

    if (rightBtn && !m_LastRightMouse)
    {
        RaycastHit hit = Raycast(m_CameraController.GetCamera().GetPosition(),
                                 m_CameraController.GetCamera().GetForward(),
                                 6.0f);
        if (hit.hit)
        {
            if (CanPlaceBlock(hit.previous))
                m_World.SetBlock(hit.previous.x, hit.previous.y, hit.previous.z, m_BlockRegistry.getID("stone"), m_BlockRegistry);
        }
    }

    m_LastLeftMouse = left;
    m_LastRightMouse = rightBtn;

    RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
    RenderCommand::Clear();

    Renderer::BeginScene(m_CameraController.GetCamera());

    m_World.Draw(m_Shader, m_BlockRegistry);

    Renderer::EndScene();
}

void GameLayer::OnImGuiRender()
{
}

void GameLayer::OnEvent(Event &event)
{
    m_CameraController.OnEvent(event);
}

glm::ivec3 GameLayer::WorldPosToBlock(const glm::vec3 &pos) const
{
    const int bx = static_cast<int>(std::floor(pos.x));
    const int bz = static_cast<int>(std::floor(pos.z));
    const int by = static_cast<int>(std::ceil(pos.y - 0.0001f));
    return {bx, by, bz};
}

bool GameLayer::CollidesAt(const glm::vec3 &pos) const
{
    const glm::vec3 min = pos + glm::vec3(-m_PlayerRadius, 0.0f, -m_PlayerRadius);
    const glm::vec3 max = pos + glm::vec3(m_PlayerRadius, m_PlayerHeight, m_PlayerRadius);

    const int minX = static_cast<int>(std::floor(min.x));
    const int maxX = static_cast<int>(std::floor(max.x - 0.0001f));
    const int minZ = static_cast<int>(std::floor(min.z));
    const int maxZ = static_cast<int>(std::floor(max.z - 0.0001f));
    const int minY = static_cast<int>(std::ceil(min.y - 0.0001f));
    const int maxY = static_cast<int>(std::ceil(max.y - 0.0001f));

    for (int y = minY; y <= maxY; y++)
        for (int x = minX; x <= maxX; x++)
            for (int z = minZ; z <= maxZ; z++)
            {
                if (!m_World.IsSolid(x, y, z, m_BlockRegistry))
                    continue;

                const glm::vec3 blockMin((float)x, (float)y - 1.0f, (float)z);
                const glm::vec3 blockMax((float)x + 1.0f, (float)y, (float)z + 1.0f);

                const bool overlap =
                    (min.x < blockMax.x && max.x > blockMin.x) &&
                    (min.y < blockMax.y && max.y > blockMin.y) &&
                    (min.z < blockMax.z && max.z > blockMin.z);

                if (overlap)
                    return true;
            }

    return false;
}

glm::vec3 GameLayer::MoveAndCollide(const glm::vec3 &pos, const glm::vec3 &delta, bool &hitY)
{
    glm::vec3 result = pos;
    glm::vec3 remaining = delta;

    const float step = 0.05f;

    auto stepAxis = [&](int axis)
    {
        float *coord = axis == 0 ? &result.x : (axis == 1 ? &result.y : &result.z);
        float remainingAxis = axis == 0 ? remaining.x : (axis == 1 ? remaining.y : remaining.z);
        if (std::fabs(remainingAxis) < 0.00001f)
            return;

        float dir = remainingAxis > 0.0f ? 1.0f : -1.0f;
        float moveLeft = std::fabs(remainingAxis);

        while (moveLeft > 0.0f)
        {
            float stepAmount = std::min(step, moveLeft);
            *coord += stepAmount * dir;

            if (CollidesAt(result))
            {
                // Undo and stop along this axis
                *coord -= stepAmount * dir;
                if (axis == 1)
                    hitY = true;
                break;
            }

            moveLeft -= stepAmount;
        }
    };

    if (delta.x != 0.0f)
        stepAxis(0);
    if (delta.z != 0.0f)
        stepAxis(2);
    if (delta.y != 0.0f)
        stepAxis(1);

    return result;
}

float GameLayer::FindGroundY(int worldX, int worldZ) const
{
    for (int y = Chunk::SIZE_Y - 1; y >= 0; y--)
    {
        if (m_World.IsSolid(worldX, y, worldZ, m_BlockRegistry))
            return (float)y;
    }
    return 1.0f;
}

GameLayer::RaycastHit GameLayer::Raycast(const glm::vec3 &origin, const glm::vec3 &dir, float maxDist) const
{
    RaycastHit hit;
    glm::vec3 direction = dir;
    if (glm::length(direction) < 0.0001f)
        return hit;

    direction = glm::normalize(direction);

    glm::ivec3 lastEmpty = WorldPosToBlock(origin);
    glm::ivec3 lastBlock = lastEmpty;

    const float step = 0.05f;
    for (float t = 0.0f; t <= maxDist; t += step)
    {
        glm::vec3 sample = origin + direction * t;
        glm::ivec3 block = WorldPosToBlock(sample);

        if (block != lastBlock)
            lastBlock = block;

        if (m_World.IsSolid(block.x, block.y, block.z, m_BlockRegistry))
        {
            hit.hit = true;
            hit.block = block;
            hit.previous = lastEmpty;
            return hit;
        }

        lastEmpty = block;
    }

    return hit;
}

bool GameLayer::CanPlaceBlock(const glm::ivec3 &block) const
{
    if (!m_World.InBoundsXZ(block.x, block.z))
        return false;
    if (block.y < 0 || block.y >= Chunk::SIZE_Y)
        return false;

    if (m_World.GetBlockID(block.x, block.y, block.z, m_BlockRegistry) != m_BlockRegistry.getID("air"))
        return false;

    const glm::vec3 blockMin((float)block.x, (float)block.y - 1.0f, (float)block.z);
    const glm::vec3 blockMax((float)block.x + 1.0f, (float)block.y, (float)block.z + 1.0f);

    const glm::vec3 min = m_PlayerPos + glm::vec3(-m_PlayerRadius, 0.0f, -m_PlayerRadius);
    const glm::vec3 max = m_PlayerPos + glm::vec3(m_PlayerRadius, m_PlayerHeight, m_PlayerRadius);

    const bool overlap =
        (min.x < blockMax.x && max.x > blockMin.x) &&
        (min.y < blockMax.y && max.y > blockMin.y) &&
        (min.z < blockMax.z && max.z > blockMin.z);

    return !overlap;
}
