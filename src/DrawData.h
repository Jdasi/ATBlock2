#pragma once

class Renderer;
class Camera;

/* Simple structure to pass to objects that require rendering.
 * This struct only contains pointers to the data it holds.
 */
struct DrawData
{
    DrawData()
        : renderer(nullptr)
        , camera(nullptr)
    {}

    Renderer* renderer;
    Camera* camera;
};
