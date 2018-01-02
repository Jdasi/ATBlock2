#pragma once

#include <vector>

/* Contains the raw level data as loaded from file.
 * Other classes may wish to use this data for visualisation and logic.
 */
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
