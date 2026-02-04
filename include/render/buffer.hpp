#pragma once
#include <cstdint>

class VertexBufferNew
{
public:
    virtual ~VertexBufferNew() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual void SetData(const void *data, uint32_t size) = 0;

    static VertexBufferNew *Create(const void *data, uint32_t size);
};
class IndexBufferNew
{
public:
    virtual ~IndexBufferNew() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    virtual uint32_t GetCount() const = 0;

    static IndexBufferNew *Create(const uint32_t *indices, uint32_t count);
};