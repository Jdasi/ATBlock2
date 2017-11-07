#include <fstream>
#include <vector>

#include "Triangle.h"


struct Vertex
{
    float x;
    float y;

    float r;
    float g;
    float b;
};

#define SafeRelease(x) if (x) { x->Release(); x = nullptr; }

Triangle::Triangle(Renderer& _renderer)
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
    SafeRelease(vertex_buffer);
    SafeRelease(vertex_shader);
    SafeRelease(pixel_shader);
    SafeRelease(input_layout);
}


void Triangle::draw(Renderer& _renderer)
{
    auto context = _renderer.getDeviceContext();

    // Bind triangle shaders.
    context->IASetInputLayout(input_layout);
    context->VSSetShader(vertex_shader, nullptr, 0);
    context->PSSetShader(pixel_shader, nullptr, 0);

    // Bind vertex buffer.
    UINT stride = sizeof(Vertex); // How far to move in memory.
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

    // Draw.
    context->Draw(3, 0);
}


void Triangle::createMesh(Renderer& _renderer)
{
    Vertex vertices[] =
    {
        { -1, -1, 1, 0, 0 },
        {  0,  1, 0, 1, 0 },
        {  1, -1, 0, 0, 1 }
    };

    // Create vertex buffer.
    auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(vertices), D3D11_BIND_VERTEX_BUFFER);
    D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
    vertex_data.pSysMem = vertices;

    _renderer.getDevice()->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
}


void Triangle::createShaders(Renderer& _renderer)
{
    using namespace std;

    // Create shaders.
    ifstream vs_file("Resources/Shaders/triangleVertexShader.cso", ios::binary);
    ifstream ps_file("Resources/Shaders/trianglePixelShader.cso", ios::binary);

    vector<char> vs_data = { istreambuf_iterator<char>(vs_file), istreambuf_iterator<char>() };
    vector<char> ps_data = { istreambuf_iterator<char>(ps_file), istreambuf_iterator<char>() };

    _renderer.getDevice()->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &vertex_shader);
    _renderer.getDevice()->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &pixel_shader);

    // Create input layouts.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    _renderer.getDevice()->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &input_layout);
}
