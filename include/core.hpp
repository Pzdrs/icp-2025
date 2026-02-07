#pragma once
#include <memory>

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;