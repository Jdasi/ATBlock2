#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <thread>

#include "SimulationSettings.h"
#include "Renderer.h"
#include "Vertex.h"
#include "VBModel.h"
#include "VBGO.h"
#include "ShaderData.h"
#include "SwarmAgent.h"
#include "NavNode.h"
#include "Level.h"

struct GameData;
struct DrawData;
class VBModelFactory;

/* Contains all the information required to drive the swarm behaviour and render
 * it to the screen.
 */
class Simulation : public AgentListener
{
public:
    Simulation(GameData* _gd, Renderer* _renderer, VBModelFactory* _vbmf);
    ~Simulation();

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
    void init(GameData* _gd, Renderer* _renderer);

    void createConstantBuffers(Renderer* _renderer);
    void createScene(Renderer* _renderer);
    void configureAgentInstanceBuffer();
    void initThreads(GameData* _gd);

    void handleInput(GameData* _gd);
    void agentBehaviourTick();
    void agentMovementTick(const float _dt);

    void drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context);
    void drawScene(ID3D11Device* _device, ID3D11DeviceContext* _context);

    void updateConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _context);
    void updateAgentInstanceBuffer();

    void updateSwarmDestination();
    bool posWithinSimBounds(const DirectX::XMFLOAT3& _pos);
    int posToTileIndex(const DirectX::XMFLOAT3& _pos);

    void generateDijkstrasDistances(const int _start_index);
    void generateFlowField();
    std::vector<NavNode*> evaluateNodeNeighbours(const int _center_tile,
        const bool _diagonals = false);

    void spawnAgents();
    void steerAgentFromNode(SwarmAgent& _agent, NavNode& _node);
    void shuntAgentFromNode(SwarmAgent& _agent, NavNode& _node);
    void keepAgentInBounds(SwarmAgent& _agent);

    void onTileIndexChanged(SwarmAgent* _agent, const int _prev_index,
        const int _new_index) override;

    SimulationSettings settings;

    Renderer* renderer;
    VBModelFactory* vbmf;

    // Constant buffer stuff.
    ID3D11Buffer* cb_gpu;           // GPU-side per-object constant buffer.
    CBPerObject* cb_cpu;            // CPU-side per-object constant buffer.

    // Swarm Agent stuff.
    ID3D11Buffer* agent_inst_buff;  // GPU buffer for drawing the agent instances.
    D3D11_BUFFER_DESC agent_inst_buff_desc;
    D3D11_SUBRESOURCE_DATA agent_inst_res_data;

    std::vector<AgentInstanceData> agent_instance_data; // Stuff given to the GPU to draw agents.
    std::vector<SwarmAgent> agents;                     // Fleshed out agent class with behaviour (uses AgentInstanceData).

    std::unique_ptr<VBModel> agent_model;

    // Scene stuff.
    ID3D11Buffer* scene_inst_buff;  // GPU buffer for drawing the node instances.

    std::vector<NavNode> nav_nodes;
    DirectX::XMMATRIX nav_world;

    std::unique_ptr<Level> level;   // Raw data of the level loaded from file.
    int grid_scale;                 // Size of the grid.
    float half_scale;               // Used to resolve tile center offsets.
    float hundredth_scale;          // Used to shift agent positions based on grid size.

    // Helper stuff.
    std::unique_ptr<VBGO> cursor;               // Shows where the agents will be spawned.
    std::unique_ptr<VBGO> waypoint_indicator;   // Shows where the swarm destination is.

    std::atomic<int> paused_flag;

    // Thread stuff.
    std::thread behaviour_thread;   // Determines movement direction of each agent.
    std::thread movement_thread;    // Actually moves each agent.

};
