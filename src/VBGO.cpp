#include <fstream>

#include "VBGO.h"


VBGO::VBGO(Renderer* _renderer)
    : GameObject()
    , vertex_buffer(nullptr)
    , index_buffer(nullptr)
    , cb_gpu(nullptr)
    , cb_cpu(nullptr)
    , vertex_shader(nullptr)
    , pixel_shader(nullptr)
    , input_layout(nullptr)
    , topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    , num_vertices(0)
    , num_indices(0)
{
    init(_renderer);
}


VBGO::~VBGO()
{
    SAFE_RELEASE(vertex_buffer);
    SAFE_RELEASE(index_buffer);
    SAFE_RELEASE(cb_gpu);
    SAFE_RELEASE(vertex_shader);
    SAFE_RELEASE(pixel_shader);
    SAFE_RELEASE(input_layout);

    if (cb_cpu)
    {
        delete cb_cpu;
        cb_cpu = nullptr;
    }
}


void VBGO::tick(GameData* _gd)
{
    GameObject::tick(_gd);
}


void VBGO::draw(DrawData* _dd)
{
    GameObject::draw(_dd);
}


void VBGO::createMesh(Renderer* _renderer)
{
    // Default VBGO doesn't create anything.
}


void VBGO::buildVB(Renderer* _renderer, const int _num_verts, Vertex* _vertices)
{
    auto device = _renderer->getDevice();

    // Create vertex buffer.
    auto vertex_buffer_desc = CD3D11_BUFFER_DESC(sizeof(_vertices), D3D11_BIND_VERTEX_BUFFER);
    ZeroMemory(&vertex_buffer_desc, sizeof(vertex_buffer_desc));

    D3D11_SUBRESOURCE_DATA vertex_data = { 0 };
    vertex_data.pSysMem = _vertices;

    vertex_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    vertex_buffer_desc.ByteWidth = sizeof(Vertex) * _num_verts;
    vertex_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertex_buffer_desc.CPUAccessFlags = 0;
    vertex_buffer_desc.MiscFlags = 0;

    device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
}


void VBGO::buildIB(Renderer* _renderer, DWORD* _indices)
{
    auto device = _renderer->getDevice();

    // Create index buffer.
    D3D11_BUFFER_DESC index_buffer_desc;
    ZeroMemory(&index_buffer_desc, sizeof(index_buffer_desc));

    index_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    index_buffer_desc.ByteWidth = sizeof(DWORD) * 2 * 3;
    index_buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    index_buffer_desc.CPUAccessFlags = 0;
    index_buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA index_data;
    index_data.pSysMem = _indices;

    device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);
}


void VBGO::init(Renderer* _renderer)
{
    createShaders(_renderer);
    createConstantBuffers(_renderer);
}


void VBGO::createShaders(Renderer* _renderer)
{
    using namespace std;
    auto device = _renderer->getDevice();

    // Create shaders.
    ifstream vs_file("Resources/Shaders/VertexShader.cso", ios::binary);
    ifstream ps_file("Resources/Shaders/PixelShader.cso", ios::binary);

    vector<char> vs_data = { istreambuf_iterator<char>(vs_file), istreambuf_iterator<char>() };
    vector<char> ps_data = { istreambuf_iterator<char>(ps_file), istreambuf_iterator<char>() };

    device->CreateVertexShader(vs_data.data(), vs_data.size(), nullptr, &vertex_shader);
    device->CreatePixelShader(ps_data.data(), ps_data.size(), nullptr, &pixel_shader);

    // Create input layouts.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    device->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &input_layout);
}


void VBGO::createConstantBuffers(Renderer* _renderer)
{
    auto device = _renderer->getDevice();

    // Create constant buffer (gpu side).
    D3D11_BUFFER_DESC cb_desc;
    ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));

    cb_desc.Usage = D3D11_USAGE_DEFAULT;
    cb_desc.ByteWidth = sizeof(ConstantBuffer);
    cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb_desc.CPUAccessFlags = 0;
    cb_desc.MiscFlags = 0;

    // CPU side constant buffer.
    cb_cpu = new ConstantBuffer();
    ZeroMemory(cb_cpu, sizeof(ConstantBuffer));

    device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
}
