#pragma once
#include <vector>
#include <glad/glad.h>
#include <assert.h>

struct VertexBufferElement
{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int getSizeOfType(unsigned int type)
    {
        switch (type)
        {
        case GL_FLOAT:
            return sizeof(float);
        }
        assert(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() : stride(0) {}

    template <typename T>
    void push(unsigned int count)
    {
        static_assert(false);
    }

    template <>
    void push<float>(unsigned int count)
    {
        elements.push_back({GL_FLOAT, count, GL_FALSE});
        stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
    }

    inline const std::vector<VertexBufferElement> &getElements() const { return elements; }
    inline unsigned int getStride() const { return stride; }

private:
    std::vector<VertexBufferElement> elements;
    unsigned int stride;
};