#include "Square.h"
#include "Renderer.h"
#include "Vertex.h"


Square::Square(Renderer* _renderer)
{
    createMesh(_renderer);
}


void Square::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex(-0.5f, -0.5f, 0),
        Vertex(-0.5f,  0.5f, 0),
        Vertex(0.5f,  0.5f, 0),
        Vertex(0.5f, -0.5f, 0),
    };
    num_vertices = ARRAYSIZE(vertices);

    DWORD indices[] =
    {
        0, 1, 2,
        0, 2, 3
    };
    num_indices = ARRAYSIZE(indices);

    buildVB(_renderer, num_vertices, vertices);
    buildIB(_renderer, indices);
}
