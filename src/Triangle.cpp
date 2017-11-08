#include <fstream>
#include <vector>

#include <DirectXMath.h>

#include "Triangle.h"
#include "Vertex.h"
#include "DXUtil.h"


Triangle::Triangle(Renderer* _renderer)
    : vertex_buffer(nullptr)
    , vertex_shader(nullptr)
    , pixel_shader(nullptr)
    , input_layout(nullptr)
{
    createMesh(_renderer);
    createShaders(_renderer);
}


Triangle::~Triangle()
{
    SAFE_RELEASE(vertex_buffer);
    SAFE_RELEASE(vertex_shader);
    SAFE_RELEASE(pixel_shader);
    SAFE_RELEASE(input_layout);
}


void Triangle::draw(Renderer* _renderer)
{
    auto context = _renderer->getDeviceContext();

    // Bind triangle shaders.
    context->IASetInputLayout(input_layout);
    context->VSSetShader(vertex_shader, nullptr, 0);
    context->PSSetShader(pixel_shader, nullptr, 0);

    // Bind vertex buffer.
    UINT stride = sizeof(Vertex); // How far to move in memory.
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw.
    context->Draw(3, 0);
}


void Triangle::createMesh(Renderer* _renderer)
{
    Vertex v[] =
    {
        Vertex( 0.0f,  0.5f, 0.5f, 1, 0, 0, 1),
        Vertex( 0.5f, -0.5f, 0.5f, 0, 1, 0, 1),
        Vertex(-0.5f, -0.5f, 0.5f, 0, 0, 1, 1)
    };
    UINT num_elems = ARRAYSIZE(v);

    // Create vertex buffer.
    auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(v), D3D11_BIND_VERTEX_BUFFER);
    ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(Vertex) * num_elems;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
    vertex_data.pSysMem = v;

    _renderer->getDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
}


void Triangle::createShaders(Renderer* _renderer)
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
