#pragma once

#include <vector>
#include <memory>

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

class Simulation : public AgentListener
{
public:
    Simulation(Renderer* _renderer, VBModelFactory* _vbmf);
    ~Simulation();

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
    void createConstantBuffers(Renderer* _renderer);
    void createScene(Renderer* _renderer);
    void configureAgentInstanceBuffer();

    void handleInput(GameData* _gd);
    void handleAgentBehaviour(GameData* _gd);

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
    DirectX::XMMATRIX agent_world = DirectX::XMMatrixIdentity(); // All agents use this world matrix.

    std::unique_ptr<VBModel> agent_model;

    // Scene stuff.
    ID3D11Buffer* scene_inst_buff;

    std::vector<NavNode> nav_nodes;
    DirectX::XMMATRIX nav_world = DirectX::XMMatrixIdentity();

    std::unique_ptr<Level> level;
    int grid_scale;

    // Helper stuff.
    std::unique_ptr<VBGO> cursor;
    std::unique_ptr<VBGO> waypoint_indicator;

    bool paused;

};
