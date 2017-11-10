#include "Cube.h"


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

    /*
    // Update constant buffer.
    DirectX::XMMATRIX wvp = DirectX::XMMatrixIdentity() * _dd->camera->getViewMat() * _dd->camera->getProjMat();

    cb_cpu->wvp = XMMatrixTranspose(wvp);

    context->UpdateSubresource(cb_gpu, 0, NULL, &cb_cpu, 0, 0);
    context->VSSetConstantBuffers(0, 1, &cb_gpu);
    */

    // Draw.
    context->DrawIndexed(num_indices, 0, 0);
}


void Cube::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
        Vertex( 0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex( 0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
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
