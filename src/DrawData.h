#pragma once

#include "Renderer.h"

struct DrawData
{
    DrawData()
        : renderer(nullptr)
    {}

    Renderer* renderer;
};
