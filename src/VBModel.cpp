#include <iostream>

#include "VBModel.h"
#include "DrawData.h"
#include "Camera.h"


VBModel::VBModel(Renderer* _renderer)
    : vertex_buffer(nullptr)
    , index_buffer(nullptr)
    , topology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    , num_vertices(0)
    , num_indices(0)
{
    shader = _renderer->getShaderData(Renderer::ShaderType::DEFAULT);

    createMesh(_renderer);
}


VBModel::~VBModel()
{
    SAFE_RELEASE(vertex_buffer);
    SAFE_RELEASE(index_buffer);
}


void VBModel::draw(DrawData* _dd)
{
    auto device = _dd->renderer->getDevice();
    auto context = _dd->renderer->getDeviceContext();

    // Bind shaders.
    context->IASetInputLayout(shader->input_layout);
    context->VSSetShader(shader->vertex_shader, nullptr, 0);
    context->PSSetShader(shader->pixel_shader, nullptr, 0);

    // Bind vertex buffer.
    UINT stride = sizeof(Vertex); // How far to move in memory.
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);
    context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(topology);

    // Draw the thing.
    context->DrawIndexed(num_indices, 0, 0);
}


ID3D11Buffer* const* VBModel::getVertexBuffer() const
{
    return &vertex_buffer;
}


ID3D11Buffer* VBModel::getIndexBuffer() const
{
    return index_buffer;
}


ShaderData* VBModel::getShader() const
{
    return shader;
}


const D3D_PRIMITIVE_TOPOLOGY& VBModel::getTopology() const
{
    return topology;
}


const UINT& VBModel::getNumVertices() const
{
    return num_vertices;
}


const UINT& VBModel::getNumIndices() const
{
    return num_indices;
}


void VBModel::createMesh(Renderer* _renderer)
{
    // TODO: replace this with loading from file ..

    auto device = _renderer->getDevice();
    auto context = _renderer->getDeviceContext();

    Vertex vertices[] =
    {
        Vertex( 0.0f,  0.5f, 0),
        Vertex( 0.5f, -0.5f, 0),
        Vertex(-0.5f, -0.5f, 0),
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


void VBModel::buildVB(Renderer* _renderer, const int _num_verts, Vertex* _vertices)
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


void VBModel::buildIB(Renderer* _renderer, DWORD* _indices)
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
