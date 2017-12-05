#pragma once

#include <vector>
#include <memory>
#include <atomic>
#include <thread>

#include "Renderer.h"
#include "Vertex.h"
#include "VBModel.h"
#include "VBGO.h"
#include "ShaderData.h"
#include "SwarmAgent.h"
#include "NavNode.h"
#include "Level.h"
#include "Constants.h"

struct GameData;
struct DrawData;
class VBModelFactory;

class Simulation : public AgentListener
{
public:
    Simulation(GameData* _gd, Renderer* _renderer, VBModelFactory* _vbmf);
    ~Simulation();

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
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

    void spawnAgent();
    void steerAgentFromNode(SwarmAgent& _agent, NavNode& _node);
    void shuntAgentFromNode(SwarmAgent& _agent, NavNode& _node);
    void keepAgentInBounds(SwarmAgent& _agent);

    void onTileIndexChanged(SwarmAgent* _agent, const int _prev_index,
        const int _new_index) override;

    Renderer* renderer;
    VBModelFactory* vbmf;

    // Constant buffer stuff.
    ID3D11Buffer* cb_gpu;
    CBPerObject* cb_cpu;

    // Swarm Agent stuff.
    ID3D11Buffer* agent_inst_buff;
    D3D11_BUFFER_DESC agent_inst_buff_desc;
    D3D11_SUBRESOURCE_DATA agent_inst_res_data;

    std::vector<AgentInstanceData> agent_instance_data; // Stuff given to the GPU to draw agents.
    std::vector<SwarmAgent> agents; // Fleshed out agent class with behaviour (uses AgentInstanceData).
    DirectX::XMMATRIX agent_world; // All agents use this world matrix.

    std::unique_ptr<VBModel> agent_model;

    // Scene stuff.
    ID3D11Buffer* scene_inst_buff;

    std::vector<NavNode> nav_nodes;
    DirectX::XMMATRIX nav_world;

    std::unique_ptr<Level> level;
    int grid_scale;
    float half_scale; // Used to resolve center origin offsets.
    float hundredth_scale; // Used to shift agent positions based on grid size.

    // Helper stuff.
    std::unique_ptr<VBGO> cursor;
    std::unique_ptr<VBGO> waypoint_indicator;

    std::atomic<int> paused_flag;

    // Thread stuff.
    std::thread behaviour_thread;
    std::thread movement_thread;

};
