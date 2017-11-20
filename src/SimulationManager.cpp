#include <iostream>

#include "SimulationManager.h"
#include "VBModelFactory.h"
#include "InputHandler.h"
#include "GameData.h"
#include "DrawData.h"
#include "Camera.h"
#include "JTime.h"
#include "FileIO.h"


SimulationManager::SimulationManager(Renderer* _renderer, VBModelFactory* _vbmf)
    : renderer(_renderer)
    , vbmf(_vbmf)
    , num_agents(10000)
    , grid_scale(10)
{
    agent_model = vbmf->createSquare(Renderer::ShaderType::INSTANCED);

    cursor = std::make_unique<VBGO>(vbmf->createSquare());
    cursor->setColor(1, 1, 0);

    waypoint_indicator = std::make_unique<VBGO>(vbmf->createSquare());
    waypoint_indicator->setColor(0, 1, 0);

    agents.assign(num_agents, SwarmAgent());
    for (int i = 0; i < num_agents; ++i)
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


SimulationManager::~SimulationManager()
{
    SAFE_RELEASE(cb_gpu);
    SAFE_RELEASE(agent_inst_buff);
    SAFE_RELEASE(scene_inst_buff);
}


void SimulationManager::tick(GameData* _gd)
{
    DirectX::XMFLOAT3 cam_pos = _gd->camera_pos;
    cursor->setPos(cam_pos.x, cam_pos.y, 0);

    cursor->tick(_gd);
    waypoint_indicator->tick(_gd);
    waypoint_indicator->setRoll(waypoint_indicator->getRoll() - 2 * JTime::getDeltaTime());
    waypoint_indicator->setScale(2 + cos(5 * JTime::getTime()));

    // Perform all swarm behaviour ..
    for (SwarmAgent& agent : agents)
    {
        agent.setSeekPos(waypoint_indicator->getPos()); // Debug seek.
        agent.tick(_gd);
    }

    setSwarmDestination(_gd);

    // Update the instance buffer after behaviour tick ..
    updateAgentInstanceBuffer();
}


void SimulationManager::draw(DrawData* _dd)
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

    _dd->renderer->setRenderStyle(Renderer::RenderStyle::SOLID);
    drawAgents(device, context);

    _dd->renderer->setRenderStyle(Renderer::RenderStyle::WIREFRAME);
    drawScene(device, context);

    // Draw seperate VBGOs last as they use their own buffers.
    _dd->renderer->setRenderStyle(Renderer::RenderStyle::SOLID);
    cursor->draw(_dd);
    waypoint_indicator->draw(_dd);
}


void SimulationManager::createConstantBuffers(Renderer* _renderer)
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
    cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }
}


void SimulationManager::createScene(Renderer* _renderer)
{
    // Load the level from file.
    level = std::make_unique<Level>(FileIO::loadLevel("level1.txt"));

    // Create the grid tiles (aka Navigation Nodes).
    int grid_size = level->width * level->height;
    nav_nodes.assign(grid_size, NavNode(grid_scale));

    // Instance buffer to render the grid tiles.
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

    for (int row = 0; row < level->height; ++row)
    {
        for (int col = 0; col < level->width; ++col)
        {
            int index = (row * level->width) + col;
            auto& node = nav_nodes[index];

            node.setNodeIndex(index);
            node.setPos(col, row, 0);
            node.setWalkable(level->data[index] != 'W');
        }
    }

    HRESULT hr = renderer->getDevice()->CreateBuffer(&scene_inst_buff_desc,
        &scene_inst_res_data, &scene_inst_buff);

    DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(grid_scale, grid_scale, 1);
    nav_world = nav_world * scale_mat;
}


void SimulationManager::configureAgentInstanceBuffer()
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


void SimulationManager::drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context)
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


void SimulationManager::drawScene(ID3D11Device* _device, ID3D11DeviceContext* _context)
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

    _context->DrawIndexedInstanced(agent_model->getNumIndices(), level->data.size(), 0, 0, 0);
}


void SimulationManager::updateConstantBuffer(ID3D11Device* /*_device*/, ID3D11DeviceContext* _context)
{
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    ZeroMemory(&mapped_resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = _context->Map(cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    memcpy(mapped_resource.pData, cb_cpu, sizeof(CBPerObject));
    _context->Unmap(cb_gpu, 0);

    _context->VSSetConstantBuffers(0, 1, &cb_gpu);
}


void SimulationManager::updateAgentInstanceBuffer()
{
    HRESULT hr = { 0 };

    SAFE_RELEASE(agent_inst_buff);
    hr = renderer->getDevice()->CreateBuffer(&agent_inst_buff_desc,
        &agent_inst_res_data, &agent_inst_buff);
}


void SimulationManager::setSwarmDestination(GameData* _gd)
{
    if (!_gd->input->getKeyDown('V'))
        return;
    
    auto& pos = cursor->getPos();
    if (!posWithinSimBounds(pos))
        return;

    float half_scale = grid_scale * 0.5f;

    auto offsetx = pos.x + half_scale;
    int ix = static_cast<int>(offsetx) / grid_scale;

    auto offsety = pos.y + half_scale;
    int iy = static_cast<int>(offsety) / grid_scale;

    int index = (iy * level->width) + ix;

    if (!nav_nodes[index].isWalkable())
        return;

    DirectX::XMFLOAT3 snap_pos { 0, 0, 0 };
    snap_pos.x = ix * grid_scale;
    snap_pos.y = iy * grid_scale;
    waypoint_indicator->setPos(snap_pos);

    std::cout << "Math index: " << index << " -- Tile index: "
              << nav_nodes[index].getNodeIndex() << std::endl;
}


bool SimulationManager::posWithinSimBounds(const DirectX::XMFLOAT3& _pos)
{
    float half_scale = grid_scale * 0.5f;

    if (_pos.x + half_scale < 0 || _pos.x + half_scale >= level->width * grid_scale ||
        _pos.y + half_scale < 0 || _pos.y + half_scale >= level->height * grid_scale)
    {
        return false;
    }

    return true;
}
