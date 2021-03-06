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
#include "SimpleTimer.h"


Simulation::Simulation(GameData* _gd, Renderer* _renderer, VBModelFactory* _vbmf)
    : settings()
    , renderer(_renderer)
    , vbmf(_vbmf)
    , cb_cpu(nullptr)
    , cb_gpu(nullptr)
    , agent_inst_buff(nullptr)
    , scene_inst_buff(nullptr)
    , nav_world(DirectX::XMMatrixIdentity())
    , grid_scale(5)
    , half_scale(static_cast<float>(grid_scale) / 2)
    , hundredth_scale(static_cast<float>(grid_scale) / 100)
    , paused_flag(0)
{
    init(_gd, _renderer);
}


Simulation::~Simulation()
{
    SAFE_RELEASE(cb_gpu);
    SAFE_RELEASE(agent_inst_buff);
    SAFE_RELEASE(scene_inst_buff);

    behaviour_thread.join();
    movement_thread.join();
}


void Simulation::tick(GameData* _gd)
{
    DirectX::XMFLOAT3 cam_pos = _gd->camera_pos;
    cursor->setPos(cam_pos.x, cam_pos.y, -1);

    // Debug visualisation stuff ..
    cursor->tick(_gd);
    waypoint_indicator->tick(_gd);

    handleInput(_gd);

    if (!paused_flag)
    {
        waypoint_indicator->setRoll(waypoint_indicator->getRoll() - 2 * JTime::getDeltaTime());
        waypoint_indicator->setScale(2 + cos(5 * JTime::getTime()));
    }

    // Update instance buffer after behaviour tick ..
    if (agent_instance_data.size() > 0)
    {
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

    if (agent_instance_data.size() > 0)
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


void Simulation::init(GameData* _gd, Renderer* _renderer)
{
    // Pre-allocate large memory storage.
    agent_instance_data.reserve(settings.getMaxAgents());
    agents.reserve(settings.getMaxAgents());

    // Set the main model to be used in the simulation.
    agent_model = vbmf->createSquare(Renderer::ShaderType::INSTANCED);

    // Debug/helper visualisations.
    cursor = std::make_unique<VBGO>(vbmf->createSquare());
    cursor->setColor(1, 1, 0);

    waypoint_indicator = std::make_unique<VBGO>(vbmf->createSquare());
    waypoint_indicator->setColor(0, 1, 0);

    // Buffer and scene stuff.
    createConstantBuffers(_renderer);
    createScene(_renderer);

    // Set the initial destination for the swarm.
    updateSwarmDestination();

    // Start the threads to produce agent movement.
    initThreads(_gd);
}


/* Create the per-object buffers used to draw the agents and nav nodes.
 */
void Simulation::createConstantBuffers(Renderer* _renderer)
{
    auto* device = _renderer->getDevice();

    // Create constant buffer (GPU side).
    D3D11_BUFFER_DESC cb_desc;
    ZeroMemory(&cb_desc, sizeof(D3D11_BUFFER_DESC));

    cb_desc.Usage = D3D11_USAGE_DYNAMIC;
    cb_desc.ByteWidth = sizeof(CBPerObject);
    cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cb_desc.MiscFlags = 0;

    // Create constant buffer (CPU side).
    cb_cpu = (CBPerObject*)_aligned_malloc(sizeof(CBPerObject), 16);
    ZeroMemory(cb_cpu, sizeof(CBPerObject));

    HRESULT hr = device->CreateBuffer(&cb_desc, NULL, &cb_gpu);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Constant Buffer" << std::endl;
    }
}


/* Create and arrange the nav nodes which form the scene visualisation and
 * navigation graph.
 */
void Simulation::createScene(Renderer* _renderer)
{
    // Load the level from file.
    level = std::make_unique<Level>(FileIO::loadLevel(settings.getLevelName()));

    // Create the grid tiles (aka Navigation Nodes).
    int grid_size = level->width * level->height;
    nav_nodes.assign(grid_size, NavNode(grid_scale, settings.getMaxAgents()));

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

    // Initialise and arrange the tiles based on level data.
    for (int row = 0; row < level->height; ++row)
    {
        for (int col = 0; col < level->width; ++col)
        {
            int index = (row * level->width) + col;
            auto& node = nav_nodes[index];

            node.setNodeIndex(index);
            node.setPos(static_cast<float>(col), static_cast<float>(row), 0);
            node.setWalkable(level->data[index] != 'W');

            node.setAdjacentNeighbours(evaluateNodeNeighbours(index));
            node.setAllNeighbours(evaluateNodeNeighbours(index, true));
        }
    }

    HRESULT hr = renderer->getDevice()->CreateBuffer(&scene_inst_buff_desc,
        &scene_inst_res_data, &scene_inst_buff);

    // Create the grid scale.
    float grid_scale_f = static_cast<float>(grid_scale);
    DirectX::XMMATRIX scale_mat = DirectX::XMMatrixScaling(grid_scale_f, grid_scale_f, 1);
    nav_world = nav_world * scale_mat;
}


void Simulation::configureAgentInstanceBuffer()
{
    ZeroMemory(&agent_inst_buff_desc, sizeof(agent_inst_buff_desc));
    agent_inst_buff_desc.Usage = D3D11_USAGE_DEFAULT;
    agent_inst_buff_desc.ByteWidth = sizeof(AgentInstanceData) * agent_instance_data.size();
    agent_inst_buff_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    agent_inst_buff_desc.CPUAccessFlags = 0;
    agent_inst_buff_desc.MiscFlags = 0;

    ZeroMemory(&agent_inst_res_data, sizeof(agent_inst_res_data));
    agent_inst_res_data.pSysMem = &agent_instance_data[0];
}


void Simulation::initThreads(GameData* _gd)
{
    behaviour_thread = std::thread([this, _gd]()
    {
        while (!_gd->exit)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            agentBehaviourTick();
        }
    });

    movement_thread = std::thread([this, _gd]()
    {
        SimpleTimer timer;
        float dt = 0;

        while (!_gd->exit)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));

            dt = timer.getTimeDifference();
            timer.reset();

            agentMovementTick(dt);
        }
    });
}


void Simulation::handleInput(GameData* _gd)
{
    if (_gd->input->getActionDown(GameAction::PAUSE))
        paused_flag ^= 1; // Toggle pause.

    // Hold V to spawn agents.
    if (agents.size() < settings.getMaxAgents() && _gd->input->getKey('V'))
        spawnAgents();

    if (_gd->input->getKeyDown('B'))
        updateSwarmDestination();
}


/* Thread 1:
 * Generates the steering force to be applied to each swarm agent.
 */
void Simulation::agentBehaviourTick()
{
    // Don't do anything if paused.
    if (paused_flag)
        return;

    for (auto& agent : agents)
    {
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
            steerAgentFromNode(agent, node);
        }

        agent.steerFromNeighbourAgents(node.getAgentBin());
    }
}


/* Thread 2:
 * Moves the agent based on their evaluated steering direction.
 * Agents are kept within the simulation bounds after a move iteration.
 */
void Simulation::agentMovementTick(const float _dt)
{
    // Don't do anything if paused.
    if (paused_flag)
        return;

    for (auto& agent : agents)
    {
        agent.tick(_dt);

        int index = posToTileIndex(agent.getPos());
        if (JHelper::validIndex(index, nav_nodes.size()))
        {
            auto& node = nav_nodes[index];
            if (!node.isWalkable())
            {
                shuntAgentFromNode(agent, node);
            }
        }

        keepAgentInBounds(agent);
    }
}


void Simulation::drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
    // Update current world.
    cb_cpu->obj_world = DirectX::XMMatrixTranspose(DirectX::XMMatrixIdentity());
    updateConstantBuffer(_device, _context);

    // Bind vertex buffer.
    UINT strides[2] = { sizeof(Vertex), sizeof(AgentInstanceData) };
    UINT offsets[2] = { 0, 0 };

    ID3D11Buffer* vert_inst_buffers[2] = { *agent_model->getVertexBuffer(), agent_inst_buff };

    _context->IASetVertexBuffers(0, 2, vert_inst_buffers, strides, offsets);
    _context->IASetIndexBuffer(agent_model->getIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
    _context->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    _context->DrawIndexedInstanced(agent_model->getNumIndices(), agent_instance_data.size(), 0, 0, 0);
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


/* Sets the new destination of the swarm, based on the current position of
 * the camera (which the cursor tracks).
 * Unwalkable destinations are ignored.
 *
 * Step 1: Steps between nodes is calculated using Dijkstra's algorithm.
 * Step 2: Flow directions between nodes are calculated.
 *
 * A visualisation is then placed at the destination position.
 */
void Simulation::updateSwarmDestination()
{
    auto& pos = cursor->getPos();
    if (!posWithinSimBounds(pos))
        return;

    int goal_index = posToTileIndex(pos);
    if (!nav_nodes[goal_index].isWalkable())
        return;

    generateDijkstrasDistances(goal_index);
    generateFlowField();

    // Put indicator in the center of the tile for visual reference.
    DirectX::XMFLOAT3 snap_pos { 0, 0, 0 };
    DirectX::XMINT2 coords = JHelper::calculateCoords(goal_index, level->width);
    snap_pos.x = static_cast<float>(coords.x * grid_scale);
    snap_pos.y = static_cast<float>(coords.y * grid_scale);
    waypoint_indicator->setPos(snap_pos);

    // Debug.
    std::cout << "Math index: " << goal_index << " -- Tile index: "
              << nav_nodes[goal_index].getNodeIndex() << std::endl;
}


// Returns true if _pos is within the simulation bounds, otherwise returns false.
bool Simulation::posWithinSimBounds(const DirectX::XMFLOAT3& _pos)
{
    if (_pos.x + half_scale < 0 || _pos.x + half_scale >= level->width * grid_scale ||
        _pos.y + half_scale < 0 || _pos.y + half_scale >= level->height * grid_scale)
    {
        return false;
    }

    return true;
}


/* Uses integer math to calculate the index of a tile based on _pos.
 * This does not necessarily return a safe array index, as it assumes an infinite
 * size grid.
 */
int Simulation::posToTileIndex(const DirectX::XMFLOAT3& _pos)
{
    auto offsetx = _pos.x + half_scale;
    int ix = static_cast<int>(offsetx) / grid_scale;

    auto offsety = _pos.y + half_scale;
    int iy = static_cast<int>(offsety) / grid_scale;

    int index = (iy * level->width) + ix;
    return index;
}


/* Generate the number of steps from all locations to the goal.
 */
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
        auto& neighbours = node->getAdjacentNeighbours();

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


/* Generate the steering direction from each node to its neighbours.
 */
void Simulation::generateFlowField()
{
    for (auto& node : nav_nodes)
    {
        node.resetFlowDir();
        if (!node.isWalkable())
            continue;

        // Allow omni-directional travel (use getAdjacentNeighbours for 4 directions).
        auto& neighbours = node.getAllNeighbours();

        NavNode* progress_node = nullptr;
        int progress = 0;

        // Find the best progressor node.
        for (auto* neighbour : neighbours)
        {
            int distance = neighbour->getDistance() - node.getDistance();

            if (distance >= progress)
                continue;

            progress_node = neighbour;
            progress = distance;
        }

        if (progress_node == nullptr)
            continue;

        // Point to it.
        DirectX::XMFLOAT3 dir = DirectX::Float3DirectionAtoB(node.getPos(), progress_node->getPos());
        node.setFlowDir(dir);
    }
}


/* Returns a list of neighbours surrounding the _center_tile.
 * _diagonals can be set to true if testing 8 directions, otherwise set to false for 4 directions.
 */
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

            // Test for 8 directions.
            int diff = abs(col - coords.x) + abs(row - coords.y);
            if (!_diagonals && diff == 2)
                continue;

            int index = JHelper::calculateIndex(col, row, level->width);
            neighbours.push_back(&nav_nodes[index]);
        }
    }

    return neighbours;
}


/* Spawns agents at the current position of the cursor (which is essentially
 * the center of the screen, or the camera's position.
 * The number of agents spawned is set through the SimulationSettings file.
 */
void Simulation::spawnAgents()
{
    auto& pos = cursor->getPos();
    if (!posWithinSimBounds(pos))
        return;

    int index = posToTileIndex(pos);
    if (!nav_nodes[index].isWalkable())
        return;

    auto& agent_settings = settings.getAgentSettings(); // Shared settings.
    for (unsigned int i = 0; i < settings.getAgentsPerSpawn(); ++i)
    {
        agent_instance_data.push_back(AgentInstanceData());
        agents.push_back(SwarmAgent(agent_instance_data[agent_instance_data.size() - 1], agent_settings));

        // Try to avoid placing agents inside of each other.
        float rand_x = static_cast<float>(rand() % grid_scale) - half_scale;
        float rand_y = static_cast<float>(rand() % grid_scale) - half_scale;

        auto& agent = agents[agents.size() - 1];
        agent.setPos(pos.x + rand_x, pos.y + rand_y, 0);
        agent.attachListener(this);
    }

    std::cout << "Agents spawned, num agents: " << agents.size() << std::endl;

    // Number of agents has changed, update the buffer.
    configureAgentInstanceBuffer();
}


/* Encourages an agent to steer away from a particular node.
 * The force applied is very aggressive to ensure its force takes priority.
 */
void Simulation::steerAgentFromNode(SwarmAgent& _agent, NavNode& _node)
{
    DirectX::XMFLOAT3 diff = DirectX::Float3SubtractBfromA(_agent.getPos(), _node.getWorldPos());
    diff = DirectX::Float3Mul(diff, 100); // Really aggressive repelling force.

    _agent.applySteer(diff);
}


/* This function assumes the _agent is within _node's bounds, and forcefully moves the agent
 * to the closest edge of the node, based on the agent's position.
 */
void Simulation::shuntAgentFromNode(SwarmAgent& _agent, NavNode& _node)
{
    BoundingBox::Edge closest_edge = _node.closestEdge(_agent.getPos());
    auto& bounds = _node.getWorldBounds();
    auto& agent_pos = _agent.getPos();

    switch (closest_edge)
    {
        case BoundingBox::TOP:      { _agent.setPos(agent_pos.x, bounds.top + hundredth_scale, 0);      } break;
        case BoundingBox::BOTTOM:   { _agent.setPos(agent_pos.x, bounds.bottom - hundredth_scale, 0);   } break;
        case BoundingBox::LEFT:     { _agent.setPos(bounds.left - hundredth_scale, agent_pos.y, 0);     } break;
        case BoundingBox::RIGHT:    { _agent.setPos(bounds.right + hundredth_scale, agent_pos.y, 0);    } break;

        default: {}
    }
}


/* Forcefully moves an agent to the inner edge of the simulation space, if it is
 * currently outside of it.
 */
void Simulation::keepAgentInBounds(SwarmAgent& _agent)
{
    auto& agent_pos = _agent.getPos();

    if (posWithinSimBounds(agent_pos))
        return;

    float left = 0 - half_scale;
    float right = left + (grid_scale * level->width);
    float bottom = 0 - half_scale;
    float top = bottom + (grid_scale * level->height);

    if (agent_pos.x < left)
    {
        _agent.setPos(left + hundredth_scale, agent_pos.y, 0);
    }
    else if (agent_pos.x >= right)
    {
        _agent.setPos(right - hundredth_scale, agent_pos.y, 0);
    }

    if (agent_pos.y < bottom)
    {
        _agent.setPos(agent_pos.x, bottom + hundredth_scale, 0);
    }
    else if (agent_pos.y >= top)
    {
        _agent.setPos(agent_pos.x, top - hundredth_scale, 0);
    }
}


// Called by an agent when its current tile index changes on a frame.
void Simulation::onTileIndexChanged(SwarmAgent* _agent, const int _prev_index,
    const int _new_index)
{
    if (JHelper::validIndex(_prev_index, nav_nodes.size()))
    {
        auto& prev_node = nav_nodes[_prev_index];
        prev_node.removeAgentPtr(_agent);
    }

    auto& new_node = nav_nodes[_new_index];
    new_node.addAgentPtr(_agent);
}
