#pragma once
#include "layer.hpp"

class LayerStack
{
public:
    LayerStack() = default;
    ~LayerStack();

    LayerStack(const LayerStack &) = delete;
    LayerStack &operator=(const LayerStack &) = delete;

    void PushLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PushOverlay(Layer *overlay);
    void PopOverlay(Layer *overlay);

    auto begin() { return m_Layers.begin(); }
    auto end() { return m_Layers.end(); }

    auto begin() const { return m_Layers.begin(); }
    auto end() const { return m_Layers.end(); }

private:
    std::vector<Layer *> m_Layers;
    size_t m_LayerInsertIndex = 0;
};