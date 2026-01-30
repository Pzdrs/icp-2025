#include "render/index_buffer.hpp"
#include <glad/glad.h>
#include <iostream>

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : count(count)
{
    glGenBuffers(1, &rendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    std::cout << "Created IndexBuffer with ID: " << rendererID << " and count: " << count << std::endl;
}

IndexBuffer::~IndexBuffer()
{
    if (rendererID != 0)
    {
        std::cout << "Destroying IndexBuffer with ID: " << rendererID << std::endl;
        glDeleteBuffers(1, &rendererID);
    }
}

IndexBuffer &IndexBuffer::operator=(IndexBuffer &&other) noexcept
{
    if (this != &other)
    {
        glDeleteBuffers(1, &rendererID); // delete old buffer

        rendererID = other.rendererID;
        count = other.count;

        other.rendererID = 0;
        other.count = 0;
    }
    return *this;
}

void IndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererID);
}

void IndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}