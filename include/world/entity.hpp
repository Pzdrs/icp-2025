#pragma once

#include <entt/entt.hpp>
#include "world/world.hpp"

class Entity
{
public:
    Entity() = default;
    Entity(entt::entity handle, World *world);
    Entity(const Entity &other) = default;

    template <typename T, typename... Args>
    T &AddComponent(Args &&...args)
    {
        ASSERT(!HasComponent<T>(), "Entity already has component!");
        T &component = m_World->m_Registry.emplace<T>(m_Handle, std::forward<Args>(args)...);
        return component;
    }

    template <typename T, typename... Args>
    T &AddOrReplaceComponent(Args &&...args)
    {
        T &component = m_World->m_Registry.emplace_or_replace<T>(m_Handle, std::forward<Args>(args)...);
        return component;
    }

    template <typename T>
    T &GetComponent()
    {
        ASSERT(HasComponent<T>(), "Entity does not have component!");
        return m_World->m_Registry.get<T>(m_Handle);
    }

    template <typename T>
    bool HasComponent()
    {
        return m_World->m_Registry.any_of<T>(m_Handle);
    }

    template <typename T>
    void RemoveComponent()
    {
        ASSERT(HasComponent<T>(), "Entity does not have component!");
        m_World->m_Registry.remove<T>(m_Handle);
    }

    operator entt::entity() const { return m_Handle; }

private:
    entt::entity m_Handle;
    World *m_World;
};