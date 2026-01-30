#include "render/vertex_buffer.hpp"
#include <glad/glad.h>
#include <iostream>

VertexBuffer::VertexBuffer(const void *data, unsigned int size)
{
    this->size = size;
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    std::cout << "Created VertexBuffer with ID: " << rendererID << " and size: " << size << std::endl;
}

VertexBuffer::~VertexBuffer()
{
    if (rendererID != 0)
    {
        std::cout << "Destroying VertexBuffer with ID: " << rendererID << std::endl;
        glDeleteBuffers(1, &rendererID);
    }
}

VertexBuffer &VertexBuffer::operator=(VertexBuffer &&other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &rendererID); // delete old buffer

        rendererID = other.rendererID;
        size = other.size;

        other.rendererID = 0;
        other.size = 0;
    }
    return *this;
}

void VertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, rendererID);
}

void VertexBuffer::unbind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}