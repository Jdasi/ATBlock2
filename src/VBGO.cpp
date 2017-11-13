#include <fstream>
#include <iostream>

#include "VBGO.h"
#include "DrawData.h"
#include "Camera.h"


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

    //if (cb_cpu)
    //{
    //    delete cb_cpu;
    //    cb_cpu = nullptr;
    //}
}


void VBGO::tick(GameData* _gd)
{
    GameObject::tick(_gd);
}


void VBGO::draw(DrawData* _dd)
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

    // Update constant buffer.
    DirectX::XMMATRIX wvp = world_mat * _dd->camera->getViewMat() * _dd->camera->getProjMat();
    cb_cpu->wvp = DirectX::XMMatrixTranspose(wvp);

    D3D11_MAPPED_SUBRESOURCE mapped_buffer;
    ZeroMemory(&mapped_buffer, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = context->Map(cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
    memcpy(mapped_buffer.pData, cb_cpu, sizeof(ConstantBuffer));
    context->Unmap(cb_gpu, 0);

    context->VSSetConstantBuffers(0, 1, &cb_gpu);

    // Draw the thing.
    context->DrawIndexed(num_indices, 0, 0);

    // Base draw.
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

    HRESULT hr = device->CreateBuffer(&vertex_buffer_desc, &vertex_data, &vertex_buffer);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Vertex Buffer" << std::endl;
    }
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

    HRESULT hr = device->CreateBuffer(&index_buffer_desc, &index_data, &index_buffer);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Index Buffer" << std::endl;
    }
}


void VBGO::init(Renderer* _renderer)
{
    createShaders(_renderer);
    createConstantBuffers(_renderer);
}


void VBGO::createShaders(Renderer* _renderer)
{
    HRESULT hr;

    using namespace std;
    auto device = _renderer->getDevice();

    // Create shaders.
    ifstream vs_file("Resources/Shaders/VertexShader.cso", ios::binary);
    ifstream ps_file("Resources/Shaders/PixelShader.cso", ios::binary);

    vector<char> vs_data = { istreambuf_iterator<char>(vs_file), istreambuf_iterator<char>() };
    vector<char> ps_data = { istreambuf_iterator<char>(ps_file), istreambuf_iterator<char>() };

    hr = device->CreateVertexShader(vs_data.data(), vs_data.size(), NULL, &vertex_shader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Vertex Shader" << std::endl;
    }

    hr = device->CreatePixelShader(ps_data.data(), ps_data.size(), NULL, &pixel_shader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Pixel Shader" << std::endl;
    }

    // Create input layouts.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    hr = device->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &input_layout);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Input Layout" << std::endl;
    }
}


void VBGO::createConstantBuffers(Renderer* _renderer)
{
    auto device = _renderer->getDevice();

    // Create constant buffer (gpu side).
    D3D11_BUFFER_DESC cb_desc;
    ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));

    cb_desc.Usage = D3D11_USAGE_DYNAMIC;
    cb_desc.ByteWidth = sizeof(ConstantBuffer);
    cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cb_desc.MiscFlags = 0;

    // CPU side constant buffer.
    cb_cpu = (ConstantBuffer*)_aligned_malloc(sizeof(ConstantBuffer), 16);
    ZeroMemory(cb_cpu, sizeof(ConstantBuffer));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }
}
