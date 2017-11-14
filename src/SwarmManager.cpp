#include <iostream>

#include "SwarmManager.h"
#include "DrawData.h"
#include "Camera.h"
#include "JTime.h"


SwarmManager::SwarmManager(Renderer* _renderer, VBModel* _agent_model, const int _num_agents)
    : renderer(_renderer)
    , agent_model(_agent_model)
    , num_agents(_num_agents)
{
    agents.assign(_num_agents, InstanceData());

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
    cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }

    DirectX::XMVECTOR temp;
    for (int i = 0; i < _num_agents; ++i)
    {
        float rand_x = ((float)(rand() % 2000) / 10) - 100;
        float rand_y = ((float)(rand() % 2000) / 10) - 100;
        temp = DirectX::XMVectorSet(rand_x, rand_y, 0, 0.0f);

        DirectX::XMStoreFloat3(&agents[i].pos, temp);
    }

    D3D11_BUFFER_DESC instBuffDesc;
    ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));

    instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    instBuffDesc.ByteWidth = sizeof(InstanceData) * _num_agents;
    instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    instBuffDesc.CPUAccessFlags = 0;
    instBuffDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA instData;
    ZeroMemory(&instData, sizeof(instData));

    instData.pSysMem = &agents[0];
    hr = _renderer->getDevice()->CreateBuffer(&instBuffDesc, &instData, &agent_instance_buff);
}


void SwarmManager::tick(GameData* _gd)
{
    HRESULT hr = { 0 };

    D3D11_BUFFER_DESC instBuffDesc;
    ZeroMemory(&instBuffDesc, sizeof(instBuffDesc));

    instBuffDesc.Usage = D3D11_USAGE_DEFAULT;
    instBuffDesc.ByteWidth = sizeof(InstanceData) * num_agents;
    instBuffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    instBuffDesc.CPUAccessFlags = 0;
    instBuffDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA instData;
    ZeroMemory(&instData, sizeof(instData));

    instData.pSysMem = &agents[0];
    SAFE_RELEASE(agent_instance_buff);
    hr = renderer->getDevice()->CreateBuffer(&instBuffDesc, &instData, &agent_instance_buff);
}


void SwarmManager::draw(DrawData* _dd)
{
    auto device = _dd->renderer->getDevice();
    auto context = _dd->renderer->getDeviceContext();

    // Bind shaders.
    auto shader = agent_model->getShader();

    context->IASetInputLayout(shader->input_layout);
    context->VSSetShader(shader->vertex_shader, nullptr, 0);
    context->PSSetShader(shader->pixel_shader, nullptr, 0);

    // Update constant buffer.
    cb_cpu->wvp = agent_world * _dd->camera->getViewMat() * _dd->camera->getProjMat();

    D3D11_MAPPED_SUBRESOURCE mapped_buffer;
    ZeroMemory(&mapped_buffer, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = context->Map(cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
    memcpy(mapped_buffer.pData, cb_cpu, sizeof(CBPerObject));
    context->Unmap(cb_gpu, 0);

    context->VSSetConstantBuffers(0, 1, &cb_gpu);

    // Bind vertex buffer.
    UINT strides[2] = { sizeof(Vertex), sizeof(InstanceData) };
    UINT offsets[2] = { 0, 0 };

    ID3D11Buffer* vertInstBuffers[2] = { *agent_model->getVertexBuffer(), agent_instance_buff };

    context->IASetVertexBuffers(0, 2, vertInstBuffers, strides, offsets);
    context->IASetIndexBuffer(agent_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexedInstanced(agent_model->getNumIndices(), num_agents, 0, 0, 0);
}
