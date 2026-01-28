#pragma once
#include "window.hpp"

class Scuffcraft
{
public:
    int init();
    void run();
    void shutdown();

private:
    Window window;
};