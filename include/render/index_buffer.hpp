#pragma once

class IndexBuffer 
{
public:
    IndexBuffer() = default;
    IndexBuffer(const unsigned int *data, unsigned int count);
    ~IndexBuffer();

    // Move constructor
    IndexBuffer(IndexBuffer &&other) noexcept
        : rendererID(other.rendererID), count(other.count)
    {
        other.rendererID = 0;
        other.count = 0;
    }

    // Move assignment
    IndexBuffer &operator=(IndexBuffer &&other) noexcept;

    // Delete copy constructor/assignment
    IndexBuffer(const IndexBuffer &) = delete;
    IndexBuffer &operator=(const IndexBuffer &) = delete;

    void bind() const;
    void unbind() const;

    inline unsigned int getCount() const { return count; }
    inline unsigned int getID() const { return rendererID; }

private:
    unsigned int rendererID;
    unsigned int count;
};