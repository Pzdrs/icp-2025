#include "pch.hpp"
#include "world/entity.hpp"

Entity::Entity(entt::entity handle, World *world)
    : m_Handle(handle), m_World(world)
{
}