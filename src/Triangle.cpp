#include "Triangle.h"


Triangle::Triangle(Renderer* _renderer)
    : VBGO(_renderer)
{
    createMesh(_renderer);
}


void Triangle::tick(GameData* _gd)
{
    VBGO::tick(_gd);
}


void Triangle::draw(DrawData* _dd)
{
    VBGO::draw(_dd);
}


void Triangle::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex( 0.0f,  0.5f, 0,      1.0f, 0.0f, 0.0f, 1.0f),
        Vertex( 0.5f, -0.5f, 0,      0.0f, 1.0f, 0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, 0,      0.0f, 0.0f, 1.0f, 1.0f),
    };
    num_vertices = ARRAYSIZE(vertices);

    DWORD indices[] =
    {
        0, 1, 2,
    };
    num_indices = ARRAYSIZE(indices);

    buildVB(_renderer, num_vertices, vertices);
    buildIB(_renderer, indices);
}
