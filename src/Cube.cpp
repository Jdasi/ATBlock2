#include <fstream>
#include <vector>

#include <DirectXMath.h>

#include "Cube.h"
#include "Vertex.h"
#include "DXUtil.h"


Cube::Cube(Renderer* _renderer)
    : vertex_buffer(nullptr)
    , index_buffer(nullptr)
    , vertex_shader(nullptr)
    , pixel_shader(nullptr)
    , input_layout(nullptr)
{
    createMesh(_renderer);
    createShaders(_renderer);
}


Cube::~Cube()
{
    SAFE_RELEASE(vertex_buffer);
    SAFE_RELEASE(index_buffer);
    SAFE_RELEASE(vertex_shader);
    SAFE_RELEASE(pixel_shader);
    SAFE_RELEASE(input_layout);
}


void Cube::draw(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    // Bind square shaders.
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
    context->DrawIndexed(6, 0, 0);
}


void Cube::createMesh(Renderer* _renderer)
{
    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex v[] =
    {
        Vertex(-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f),
        Vertex(-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
        Vertex(0.5f,  0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f),
        Vertex(0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f),
    };
    UINT num_elems = ARRAYSIZE(v);

    DWORD indices[] =
    {
        0, 1, 2,
        0, 2, 3,
    };

    // Create vertex buffer.
    auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(v), D3D11_BIND_VERTEX_BUFFER);
    ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(Vertex) * num_elems;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;

    // Create index buffer.
    D3D11_BUFFER_DESC index_buffer_desc;
    ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));

    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(DWORD) * 2 * 3;
    index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buffer_desc.CPUAccessFlags = 0;
    index_buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
    vertex_data.pSysMem = v;

    D3D11_SUBRESOURCE_DATA index_data;
    index_data.pSysMem = indices;

    device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);
    device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
}


void Cube::createShaders(Renderer* _renderer)
{
    using namespace std;

    // Create shaders.
    ifstream vs_file("Resources/Shaders/VertexShader.cso", ios::binary);
    ifstream ps_file("Resources/Shaders/PixelShader.cso", ios::binary);

    vector<char> vs_data = { istreambuf_iterator<char>(vs_file), istreambuf_iterator<char>() };
    vector<char> ps_data = { istreambuf_iterator<char>(ps_file), istreambuf_iterator<char>() };

    _renderer->getDevice()->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &vertex_shader);
    _renderer->getDevice()->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &pixel_shader);

    // Create input layouts.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    _renderer->getDevice()->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &input_layout);
}
