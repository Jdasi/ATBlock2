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
    createConstantBuffers(_renderer);

    agents.assign(_num_agents, SwarmAgent());
    for (int i = 0; i < _num_agents; ++i)
    {
        float rand_x = ((float)(rand() % 2000) / 10) - 100;
        float rand_y = ((float)(rand() % 2000) / 10) - 100;
        agents[i].setPos(rand_x, rand_y, 0);

        float rand_z = ((float)(rand() % 2000) / 10) - 100;
        agents[i].setColor(rand_x, rand_y, rand_z, 1);
    }
}


void SwarmManager::tick(GameData* _gd)
{
    // Perform all swarm behaviour ..
    for (SwarmAgent& agent : agents)
    {
        agent.adjustPos(0, 2 * JTime::getDeltaTime(), 0);
    }

    // Update the instance buffer ..
    updateInstanceBuffer();
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
    UINT strides[2] = { sizeof(Vertex), sizeof(SwarmAgent) };
    UINT offsets[2] = { 0, 0 };

    ID3D11Buffer* vertInstBuffers[2] = { *agent_model->getVertexBuffer(), agent_instance_buff };

    context->IASetVertexBuffers(0, 2, vertInstBuffers, strides, offsets);
    context->IASetIndexBuffer(agent_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    context->DrawIndexedInstanced(agent_model->getNumIndices(), num_agents, 0, 0, 0);
}


void SwarmManager::createConstantBuffers(Renderer* _renderer)
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

    // CPU side constant buffer.
    cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }
}


void SwarmManager::updateInstanceBuffer()
{
    HRESULT hr = { 0 };

    D3D11_BUFFER_DESC instance_buffer_desc;
    ZeroMemory(&instance_buffer_desc, sizeof(instance_buffer_desc));

    instance_buffer_desc.Usage = D3D11_USAGE_DEFAULT;
    instance_buffer_desc.ByteWidth = sizeof(SwarmAgent) * num_agents;
    instance_buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    instance_buffer_desc.CPUAccessFlags = 0;
    instance_buffer_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA instData;
    ZeroMemory(&instData, sizeof(instData));

    instData.pSysMem = &agents[0];
    SAFE_RELEASE(agent_instance_buff);
    hr = renderer->getDevice()->CreateBuffer(&instance_buffer_desc, &instData, &agent_instance_buff);
}