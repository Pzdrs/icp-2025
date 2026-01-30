#pragma once

// storing raw vertex data in GPU memory
class VertexBuffer
{
public:
    VertexBuffer() = default;
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();

    // Move constructor
    VertexBuffer(VertexBuffer &&other) noexcept
        : rendererID(other.rendererID), size(other.size)
    {
        other.rendererID = 0;
        other.size = 0;
    }

    // Move assignment
    VertexBuffer &operator=(VertexBuffer &&other) noexcept;


    // Delete copy constructor/assignment
    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete;

    void bind() const;
    void unbind() const;

    inline unsigned int getID() const { return rendererID; }
    inline unsigned int getSize() const { return size; }

private:
    unsigned int rendererID;
    unsigned int size;
};