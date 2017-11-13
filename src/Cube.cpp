#include "Cube.h"
#include "Camera.h"


Cube::Cube(Renderer* _renderer)
    : VBGO(_renderer)
{
    createMesh(_renderer);
}


void Cube::tick(GameData* _gd)
{
    VBGO::tick(_gd);
}


void Cube::draw(DrawData* _dd)
{
    VBGO::draw(_dd);
}


void Cube::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex(-0.5f, -0.5f, 0.4f,     1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.4f,     0.0f, 1.0f, 0.0f, 1.0f),
        Vertex( 0.5f,  0.5f, 0.4f,     0.0f, 0.0f, 1.0f, 1.0f),
        Vertex( 0.5f, -0.5f, 0.4f,     0.0f, 1.0f, 0.0f, 1.0f),
    };
    num_vertices = ARRAYSIZE(vertices);

    DWORD indices[] =
    {
        0, 1, 2,
        0, 2, 3,
    };
    num_indices = ARRAYSIZE(indices);

    buildVB(_renderer, num_vertices, vertices);
    buildIB(_renderer, indices);
}
