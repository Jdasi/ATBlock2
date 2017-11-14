#include <iostream>

#include "VBGO.h"
#include "DrawData.h"
#include "Camera.h"


ID3D11Buffer* VBGO::s_cb_gpu = nullptr;
CBPerObject* VBGO::s_cb_cpu = nullptr;


VBGO::VBGO(VBModel* _model)
    : GameObject()
    , model(_model)
{
}


void VBGO::tick(GameData* _gd)
{
    GameObject::tick(_gd);
}


void VBGO::draw(DrawData* _dd)
{
    auto device = _dd->renderer->getDevice();
    auto context = _dd->renderer->getDeviceContext();

    // Update constant buffer.
    s_cb_cpu->wvp = getWorldMat() * _dd->camera->getViewMat() * _dd->camera->getProjMat();

    D3D11_MAPPED_SUBRESOURCE mapped_buffer;
    ZeroMemory(&mapped_buffer, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = context->Map(s_cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
    memcpy(mapped_buffer.pData, s_cb_cpu, sizeof(CBPerObject));
    context->Unmap(s_cb_gpu, 0);

    context->VSSetConstantBuffers(0, 1, &s_cb_gpu);

    // Draw the thing.
    model->draw(_dd);
}


void VBGO::init(Renderer* _renderer)
{
    auto device = _renderer->getDevice();

    // Create constant buffer (gpu side).
    D3D11_BUFFER_DESC cb_desc;
    ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));

    cb_desc.Usage = D3D11_USAGE_DYNAMIC;
    cb_desc.ByteWidth = sizeof(CBPerObject);
    cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb_desc.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
    cb_desc.MiscFlags = 0;

    // CPU side constant buffer.
    s_cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(s_cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &s_cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }
}


void VBGO::cleanUp()
{
    SAFE_RELEASE(s_cb_gpu);
}
