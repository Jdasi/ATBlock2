#include <iostream>

#include "SwarmManager.h"
#include "DrawData.h"
#include "Camera.h"
#include "JTime.h"


SwarmManager::SwarmManager(Renderer* _renderer, VBModel* _agent_model, const int _num_agents)
    : renderer(_renderer)
    , agent_model(_agent_model)
    , num_agents(_num_agents)
    , grid_size_x(1)
    , grid_size_y(1)
    , grid_scale(10)
{
    agents.assign(_num_agents, SwarmAgent());
    for (int i = 0; i < _num_agents; ++i)
    {
        float rand_x = ((float)(rand() % 2000) / 10) - 100;
        float rand_y = ((float)(rand() % 2000) / 10) - 100;

        agents[i].setPos(rand_x, rand_y, 0);
        agents[i].setColor(1, 0, 0, 1);
    }

    createConstantBuffers(_renderer);
    createScene(_renderer);
    configureAgentInstanceBuffer();
}


SwarmManager::~SwarmManager()
{
    SAFE_RELEASE(cb_gpu);
    SAFE_RELEASE(agent_inst_buff);
    SAFE_RELEASE(scene_inst_buff);
}


void SwarmManager::tick(GameData* _gd)
{
    /*
    // Perform all swarm behaviour ..
    for (SwarmAgent& agent : agents)
    {
        agent.adjustPos(0, 2 * JTime::getDeltaTime(), 0);
    }
    */

    DirectX::XMFLOAT3 cam_pos = _gd->camera_pos;
    agents[0].setPos(DirectX::XMFLOAT3(cam_pos.x, cam_pos.y, 0));

    nav_nodes[0].containsPoint(agents[0].getPos());

    // Update the instance buffer after behaviour tick ..
    updateAgentInstanceBuffer();
}


void SwarmManager::draw(DrawData* _dd)
{
    auto* device = _dd->renderer->getDevice();
    auto* context = _dd->renderer->getDeviceContext();

    // Bind shaders.
    auto shader = agent_model->getShader();

    context->IASetInputLayout(shader->input_layout);
    context->VSSetShader(shader->vertex_shader, nullptr, 0);
    context->PSSetShader(shader->pixel_shader, nullptr, 0);

    // START CONSTANT BUFFER STUFF ---------------------------------------------
    cb_cpu->view = DirectX::XMMatrixTranspose(_dd->camera->getViewMat());
    cb_cpu->proj = DirectX::XMMatrixTranspose(_dd->camera->getProjMat());
    // END CONSTANT BUFFER STUFF -----------------------------------------------

    drawAgents(device, context);
    drawScene(device, context);
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


void SwarmManager::createScene(Renderer* _renderer)
{
    int grid_size = grid_size_x * grid_size_y;

    nav_nodes.assign(grid_size, NavNode(grid_scale));

    D3D11_BUFFER_DESC scene_inst_buff_desc;
    ZeroMemory(&scene_inst_buff_desc, sizeof(scene_inst_buff_desc));
    scene_inst_buff_desc.Usage = D3D11_USAGE_DEFAULT;
    scene_inst_buff_desc.ByteWidth = sizeof(NavNode) * grid_size;
    scene_inst_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    scene_inst_buff_desc.CPUAccessFlags = 0;
    scene_inst_buff_desc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA scene_inst_res_data;
    ZeroMemory(&scene_inst_res_data, sizeof(scene_inst_res_data));
    scene_inst_res_data.pSysMem = &nav_nodes[0];

    for (int row = 0; row < grid_size_y; ++row)
    {
        for (int col = 0; col < grid_size_x; ++col)
        {
            auto& node = nav_nodes[(row * grid_size_x) + col];
            node.setPos(col, row, 0);
            node.setColor(1, 1, 1, 1);
        }
    }

    HRESULT hr = renderer->getDevice()->CreateBuffer(&scene_inst_buff_desc,
        &scene_inst_res_data, &scene_inst_buff);

    DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(grid_scale, grid_scale, 1);
    nav_world = nav_world * scale_mat;
}


void SwarmManager::configureAgentInstanceBuffer()
{
    ZeroMemory(&agent_inst_buff_desc, sizeof(agent_inst_buff_desc));
    agent_inst_buff_desc.Usage = D3D11_USAGE_DEFAULT;
    agent_inst_buff_desc.ByteWidth = sizeof(SwarmAgent) * num_agents;
    agent_inst_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    agent_inst_buff_desc.CPUAccessFlags = 0;
    agent_inst_buff_desc.MiscFlags = 0;

    ZeroMemory(&agent_inst_res_data, sizeof(agent_inst_res_data));
    agent_inst_res_data.pSysMem = &agents[0];
}


void SwarmManager::drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
    // Update current world.
    cb_cpu->obj_world = DirectX::XMMatrixTranspose(agent_world);
    updateConstantBuffer(_device, _context);

    // Bind vertex buffer.
    UINT strides[2] = { sizeof(Vertex), sizeof(SwarmAgent) };
    UINT offsets[2] = { 0, 0 };

    ID3D11Buffer* vert_inst_buffers[2] = { *agent_model->getVertexBuffer(), agent_inst_buff };

    _context->IASetVertexBuffers(0, 2, vert_inst_buffers, strides, offsets);
    _context->IASetIndexBuffer(agent_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    _context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _context->DrawIndexedInstanced(agent_model->getNumIndices(), num_agents, 0, 0, 0);
}


void SwarmManager::drawScene(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
    // Update current world.
    cb_cpu->obj_world = DirectX::XMMatrixTranspose(nav_world);
    updateConstantBuffer(_device, _context);

    // Bind vertex buffer.
    UINT strides[2] = { sizeof(Vertex), sizeof(NavNode) };
    UINT offsets[2] = { 0, 0 };

    ID3D11Buffer* vert_inst_buffers[2] = { *agent_model->getVertexBuffer(), scene_inst_buff };

    _context->IASetVertexBuffers(0, 2, vert_inst_buffers, strides, offsets);
    _context->IASetIndexBuffer(agent_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    _context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _context->DrawIndexedInstanced(agent_model->getNumIndices(), grid_size_x * grid_size_y, 0, 0, 0);
}


void SwarmManager::updateConstantBuffer(ID3D11Device* /*_device*/, ID3D11DeviceContext* _context)
{
    D3D11_MAPPED_SUBRESOURCE mapped_buffer;
    ZeroMemory(&mapped_buffer, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = _context->Map(cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_buffer);
    memcpy(mapped_buffer.pData, cb_cpu, sizeof(CBPerObject));
    _context->Unmap(cb_gpu, 0);

    _context->VSSetConstantBuffers(0, 1, &cb_gpu);
}


void SwarmManager::updateAgentInstanceBuffer()
{
    HRESULT hr = { 0 };

    SAFE_RELEASE(agent_inst_buff);
    hr = renderer->getDevice()->CreateBuffer(&agent_inst_buff_desc,
        &agent_inst_res_data, &agent_inst_buff);
}
