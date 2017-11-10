#pragma once

class Renderer;
class Camera;

struct DrawData
{
    DrawData()
        : renderer(nullptr)
        , camera(nullptr)
    {}

    Renderer* renderer;
    Camera* camera;
};
