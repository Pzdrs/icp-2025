#include "render/vertex_array.hpp"
#include "render/buffer.hpp"
#include <iostream>

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &rendererID);
    std::cout << "Created VertexArray with ID: " << rendererID << std::endl;
}

VertexArray::~VertexArray()
{
    std::cout << "Destroying VertexArray with ID: " << rendererID << std::endl;
    glDeleteVertexArrays(1, &rendererID);
}

void VertexArray::bind() const
{
    glBindVertexArray(rendererID);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
{
    bind();
    vb.Bind();
    const auto &elements = layout.getElements();
    size_t offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++)
    {
        const auto &element = elements[i];
        glVertexAttribPointer(
            i, element.count, element.type,
            element.normalized,
            layout.getStride(), (const void *)offset);
        glEnableVertexAttribArray(i);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}