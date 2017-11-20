#include <iostream>

#include "VBGO.h"
#include "DXUtil.h"
#include "Camera.h"
#include "Renderer.h"
#include "Vertex.h"
#include "DrawData.h"


ID3D11Buffer* VBGO::s_cb_gpu = nullptr;
CBPerObject* VBGO::s_cb_cpu = nullptr;


VBGO::VBGO(std::unique_ptr<VBModel> _model)
    : model(std::move(_model))
{
}


void VBGO::tick(GameData* _gd)
{
    GameObject::tick(_gd);
}


void VBGO::draw(DrawData* _dd)
{
    auto context = _dd->renderer->getDeviceContext();

    // Update constant buffer.
    s_cb_cpu->obj_world = DirectX::XMMatrixTranspose(getWorldMat());
    s_cb_cpu->obj_rot = DirectX::XMMatrixTranspose(getRotMat());
    s_cb_cpu->obj_color = color;

    // Pass to GPU.
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    ZeroMemory(&mapped_resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = context->Map(s_cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    memcpy(mapped_resource.pData, s_cb_cpu, sizeof(CBPerObject));
    context->Unmap(s_cb_gpu, 0);

    context->VSSetConstantBuffers(0, 1, &s_cb_gpu);

    // Draw.
    model->draw(_dd);
}


const DirectX::XMFLOAT4& VBGO::getColor() const
{
    return color;
}


void VBGO::setColor(const DirectX::XMFLOAT4& _color)
{
    color = _color;
}


void VBGO::setColor(const float _r, const float _g, const float _b, const float _a)
{
    setColor(DirectX::XMFLOAT4(_r, _g, _b, _a));
}


void VBGO::setColor(const float _r, const float _g, const float _b)
{
    setColor(_r, _g, _b, 1);
}


bool VBGO::init(Renderer* _renderer)
{
    auto device = _renderer->getDevice();

    // Create constant buffer (gpu side).
    D3D11_BUFFER_DESC cb_desc;
    ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));

    cb_desc.Usage = D3D11_USAGE_DYNAMIC;
    cb_desc.ByteWidth = sizeof(CBPerObject);
    cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb_desc.MiscFlags = 0;

    // Create constant buffer (cpu side).
    s_cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(s_cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &s_cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create VBGO Constant Buffer" << std::endl;
        return false;
    }

    return true;
}


void VBGO::updateConstantBuffers(DrawData* _dd)
{
    s_cb_cpu->view =  DirectX::XMMatrixTranspose(_dd->camera->getViewMat());
    s_cb_cpu->proj = DirectX::XMMatrixTranspose(_dd->camera->getProjMat());
}


void VBGO::cleanUp()
{
    SAFE_RELEASE(s_cb_gpu);
}
