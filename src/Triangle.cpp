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
    auto device = _dd->renderer->getDevice();
    auto context = _dd->renderer->getDeviceContext();

    // Bind shaders.
    context->IASetInputLayout(input_layout);
    context->VSSetShader(vertex_shader, nullptr, 0);
    context->PSSetShader(pixel_shader, nullptr, 0);

    // Bind vertex buffer.
    UINT stride = sizeof(Vertex); // How far to move in memory.
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
    context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw.
    context->DrawIndexed(num_indices, 0, 0);
}


void Triangle::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex( 0.0f,  0.5f, 0.5f,      1.0f, 0.0f, 0.0f, 1.0f),
        Vertex( 0.5f, -0.5f, 0.5f,      0.0f, 1.0f, 0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, 0.5f,      0.0f, 0.0f, 1.0f, 1.0f),
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
