#pragma once

#include <vector>

struct Level
{
    Level()
        : width(0)
        , height(0)
    {
    }

    int width;
    int height;

    std::vector<char> data;
};
