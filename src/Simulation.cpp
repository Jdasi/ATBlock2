#include <iostream>

#include "Simulation.h"
#include "VBModelFactory.h"
#include "InputHandler.h"
#include "GameData.h"
#include "DrawData.h"
#include "Camera.h"
#include "JTime.h"
#include "JHelper.h"
#include "FileIO.h"


Simulation::Simulation(Renderer* _renderer, VBModelFactory* _vbmf)
    : renderer(_renderer)
    , vbmf(_vbmf)
    , grid_scale(10)
{
    agent_model = vbmf->createSquare(Renderer::ShaderType::INSTANCED);

    cursor = std::make_unique<VBGO>(vbmf->createSquare());
    cursor->setColor(1, 1, 0);

    waypoint_indicator = std::make_unique<VBGO>(vbmf->createSquare());
    waypoint_indicator->setColor(0, 1, 0);

    createConstantBuffers(_renderer);
    createScene(_renderer);

    updateSwarmDestination();
}


Simulation::~Simulation()
{
    SAFE_RELEASE(cb_gpu);
    SAFE_RELEASE(agent_inst_buff);
    SAFE_RELEASE(scene_inst_buff);
}


void Simulation::tick(GameData* _gd)
{
    DirectX::XMFLOAT3 cam_pos = _gd->camera_pos;
    cursor->setPos(cam_pos.x, cam_pos.y, -1);

    cursor->tick(_gd);
    waypoint_indicator->tick(_gd);
    waypoint_indicator->setRoll(waypoint_indicator->getRoll() - 2 * JTime::getDeltaTime());
    waypoint_indicator->setScale(2 + cos(5 * JTime::getTime()));

    // Perform all swarm behaviour ..
    for (SwarmAgent& agent : agents)
    {
        agent.tick(_gd);

        int index = posToTileIndex(agent.getPos());
        if (!JHelper::validIndex(index, nav_nodes.size()))
            continue;

        auto& node = nav_nodes[index];
        agent.setCurrentTileIndex(index);

        if (node.isWalkable())
        {
            agent.applySteer(node.getFlowDir());
        }
        else
        {
            shuntAgentFromNode(agent, node);
        }
    }

    if (_gd->input->getKey('V'))
        spawnAgent();

    if (_gd->input->getKeyDown('B'))
        updateSwarmDestination();

    if (agents.size() > 0)
    {
        // Update the instance buffer after behaviour tick ..
        updateAgentInstanceBuffer();
    }
}


void Simulation::draw(DrawData* _dd)
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

    if (agents.size() > 0)
    {
        _dd->renderer->setRenderStyle(Renderer::RenderStyle::SOLID);
        drawAgents(device, context);
    }

    _dd->renderer->setRenderStyle(Renderer::RenderStyle::WIREFRAME);
    drawScene(device, context);

    // Draw seperate VBGOs last as they use their own buffers.
    _dd->renderer->setRenderStyle(Renderer::RenderStyle::SOLID);
    cursor->draw(_dd);
    waypoint_indicator->draw(_dd);
}


void Simulation::createConstantBuffers(Renderer* _renderer)
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


void Simulation::createScene(Renderer* _renderer)
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

            node.setAdjacentNeighbours(evaluateNodeNeighbours(index));
            node.setAllNeighbours(evaluateNodeNeighbours(index, true));
        }
    }

    HRESULT hr = renderer->getDevice()->CreateBuffer(&scene_inst_buff_desc,
        &scene_inst_res_data, &scene_inst_buff);

    DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(grid_scale, grid_scale, 1);
    nav_world = nav_world * scale_mat;
}


void Simulation::configureAgentInstanceBuffer()
{
    ZeroMemory(&agent_inst_buff_desc, sizeof(agent_inst_buff_desc));
    agent_inst_buff_desc.Usage = D3D11_USAGE_DEFAULT;
    agent_inst_buff_desc.ByteWidth = sizeof(SwarmAgent) * agents.size();
    agent_inst_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    agent_inst_buff_desc.CPUAccessFlags = 0;
    agent_inst_buff_desc.MiscFlags = 0;

    ZeroMemory(&agent_inst_res_data, sizeof(agent_inst_res_data));
    agent_inst_res_data.pSysMem = &agents[0];
}


void Simulation::drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context)
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

    _context->DrawIndexedInstanced(agent_model->getNumIndices(), agents.size(), 0, 0, 0);
}


void Simulation::drawScene(ID3D11Device* _device, ID3D11DeviceContext* _context)
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


void Simulation::updateConstantBuffer(ID3D11Device* /*_device*/, ID3D11DeviceContext* _context)
{
    D3D11_MAPPED_SUBRESOURCE mapped_resource;
    ZeroMemory(&mapped_resource, sizeof(D3D11_MAPPED_SUBRESOURCE));

    HRESULT hr = _context->Map(cb_gpu, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
    memcpy(mapped_resource.pData, cb_cpu, sizeof(CBPerObject));
    _context->Unmap(cb_gpu, 0);

    _context->VSSetConstantBuffers(0, 1, &cb_gpu);
}


void Simulation::updateAgentInstanceBuffer()
{
    HRESULT hr = { 0 };

    SAFE_RELEASE(agent_inst_buff);
    hr = renderer->getDevice()->CreateBuffer(&agent_inst_buff_desc,
        &agent_inst_res_data, &agent_inst_buff);

    if (FAILED(hr))
    {
        std::cout << "Failed to update Agent Instance Buffer\n";
    }
}


void Simulation::updateSwarmDestination()
{
    auto& pos = cursor->getPos();
    if (!posWithinSimBounds(pos))
        return;

    int index = posToTileIndex(pos);
    if (!nav_nodes[index].isWalkable())
        return;

    generateDijkstrasDistances(index);
    generateFlowField();

    // Put indicator in the center of the tile for visual reference.
    DirectX::XMFLOAT3 snap_pos { 0, 0, 0 };
    DirectX::XMINT2 coords = JHelper::calculateCoords(index, level->width);
    snap_pos.x = coords.x * grid_scale;
    snap_pos.y = coords.y * grid_scale;
    waypoint_indicator->setPos(snap_pos);

    std::cout << "Math index: " << index << " -- Tile index: "
              << nav_nodes[index].getNodeIndex() << std::endl;
}


bool Simulation::posWithinSimBounds(const DirectX::XMFLOAT3& _pos)
{
    float half_scale = grid_scale * 0.5f;

    if (_pos.x + half_scale < 0 || _pos.x + half_scale >= level->width * grid_scale ||
        _pos.y + half_scale < 0 || _pos.y + half_scale >= level->height * grid_scale)
    {
        return false;
    }

    return true;
}


int Simulation::posToTileIndex(const DirectX::XMFLOAT3& _pos)
{
    float half_scale = grid_scale * 0.5f;

    auto offsetx = _pos.x + half_scale;
    int ix = static_cast<int>(offsetx) / grid_scale;

    auto offsety = _pos.y + half_scale;
    int iy = static_cast<int>(offsety) / grid_scale;

    int index = (iy * level->width) + ix;
    return index;
}


void Simulation::generateDijkstrasDistances(const int _goal_index)
{
    int UNVISITED = 99999;
    int num_walkable = 0;

    // Reset grid ready for new evaluation.
    for (auto& node : nav_nodes)
    {
        node.setDistance(UNVISITED);

        if (node.isWalkable())
            ++num_walkable;
    }

    // Set up starting node & visit list.
    auto& start_node = nav_nodes[_goal_index];
    start_node.setDistance(0);

    std::vector<NavNode*> to_visit;
    to_visit.reserve(num_walkable);
    to_visit.push_back(&start_node);

    // Evaluate distances.
    for (int i = 0; i < num_walkable; ++i)
    {
        auto* node = to_visit[i];
        auto neighbours = node->getAdjacentNeighbours();

        for (auto* neighbour : neighbours)
        {
            if (!neighbour->isWalkable() ||
                neighbour->getDistance() != UNVISITED)
            {
                continue;
            }

            neighbour->setDistance(node->getDistance() + 1);
            to_visit.push_back(neighbour);
        }
    }
}


void Simulation::generateFlowField()
{
    for (auto& node : nav_nodes)
    {
        node.resetFlowDir();
        if (!node.isWalkable())
            continue;

        auto neighbours = node.getAllNeighbours();

        NavNode* progress_node = nullptr;
        int progress = 0;

        for (NavNode* neighbour : neighbours)
        {
            int distance = neighbour->getDistance() - node.getDistance();

            if (distance >= progress)
                continue;

            progress_node = neighbour;
            progress = distance;
        }

        if (progress_node == nullptr)
            continue;

        DirectX::XMFLOAT3 dir = DirectX::Float3DirectionAtoB(
            node.getPos(), progress_node->getPos());
        node.setFlowDir(dir);
    }
}


std::vector<NavNode*> Simulation::evaluateNodeNeighbours(const int _center_tile,
    const bool _diagonals)
{
    std::vector<NavNode*> neighbours;
    neighbours.reserve(_diagonals ? 8 : 4);

    auto coords = JHelper::calculateCoords(_center_tile, level->width);

    for (int row = coords.y - 1; row <= coords.y + 1; ++row)
    {
        for (int col = coords.x - 1; col <= coords.x + 1; ++col)
        {
            if ((col < 0 || col >= level->width) ||
                (row < 0 || row >= level->height) ||
                (col == coords.x && row == coords.y))
            {
                continue;
            }

            int diff = abs(col - coords.x) + abs(row - coords.y);
            if (!_diagonals && diff == 2)
                continue;

            int index = JHelper::calculateIndex(col, row, level->width);
            neighbours.push_back(&nav_nodes[index]);
        }
    }

    return neighbours;
}


void Simulation::spawnAgent()
{
    auto& pos = cursor->getPos();
    if (!posWithinSimBounds(pos))
        return;

    int index = posToTileIndex(pos);
    if (!nav_nodes[index].isWalkable())
        return;

    agents.push_back(SwarmAgent());

    auto& agent = agents[agents.size() - 1];
    agent.setPos(pos.x, pos.y, 0);
    agent.setColor(1, 0, 0, 1);

    std::cout << "Agent spawned, num agents: " << agents.size() << std::endl;;

    configureAgentInstanceBuffer();
}


void Simulation::shuntAgentFromNode(SwarmAgent& _agent, NavNode& _node)
{
    DirectX::XMFLOAT3 diff = DirectX::Float3SubtractBfromA(_agent.getPos(), _node.getWorldPos());
    diff = DirectX::Float3Normalized(diff);

    _agent.adjustPos(diff);
}
