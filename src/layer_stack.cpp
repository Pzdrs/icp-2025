#include "layer_stack.hpp"

LayerStack::~LayerStack()
{
    for (Layer* layer : m_Layers)
        delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
    m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
    ++m_LayerInsertIndex;
}

void LayerStack::PushOverlay(Layer* overlay)
{
    m_Layers.emplace_back(overlay);
}

void LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
    if (it != m_Layers.end())
    {
        size_t index = it - m_Layers.begin();
        if (index < m_LayerInsertIndex)
            --m_LayerInsertIndex;

        m_Layers.erase(it);
    }
}

void LayerStack::PopOverlay(Layer* overlay)
{
    auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
    if (it != m_Layers.end())
        m_Layers.erase(it);
}
