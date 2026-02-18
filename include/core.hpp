#pragma once
#include <cstdint>
#include <glm/glm.hpp>

#define BIND_EVENT_FN(fn) [this](auto &&...args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace std
{
    template <>
    struct hash<glm::ivec2>
    {
        size_t operator()(const glm::ivec2 &v) const noexcept
        {
            // 64-bit mix (very important for world coords)
            uint64_t x = static_cast<uint32_t>(v.x);
            uint64_t y = static_cast<uint32_t>(v.y);

            return (x << 32) ^ y;
        }
    };
}

template <typename T>
using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template <typename T>
using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

using GeneratorSeed = uint64_t;