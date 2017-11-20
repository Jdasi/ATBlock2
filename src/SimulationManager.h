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

class SimulationManager
{
public:
    SimulationManager(Renderer* _renderer, VBModelFactory* _vbmf);
    ~SimulationManager();

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
    void createConstantBuffers(Renderer* _renderer);
    void createScene(Renderer* _renderer);
    void configureAgentInstanceBuffer();

    void drawAgents(ID3D11Device* _device, ID3D11DeviceContext* _context);
    void drawScene(ID3D11Device* _device, ID3D11DeviceContext* _context);

    void updateConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _context);
    void updateAgentInstanceBuffer();

    void setSwarmDestination(GameData* _gd);
    bool posWithinSimBounds(const DirectX::XMFLOAT3& _pos);
    void processDijkstrasAlgorithm(const int _start_index);
    std::vector<NavNode*> getNodeNeighbours(const int _center_tile);

    Renderer* renderer;
    VBModelFactory* vbmf;

    // Constant buffer stuff.
    ID3D11Buffer* cb_gpu;
    CBPerObject* cb_cpu;

    // Swarm Agent stuff.
    ID3D11Buffer* agent_inst_buff;
    D3D11_BUFFER_DESC agent_inst_buff_desc;
    D3D11_SUBRESOURCE_DATA agent_inst_res_data;

    std::vector<SwarmAgent> agents;
    DirectX::XMMATRIX agent_world = DirectX::XMMatrixIdentity();

    std::unique_ptr<VBModel> agent_model;
    int num_agents;

    // Scene stuff.
    ID3D11Buffer* scene_inst_buff;

    std::vector<NavNode> nav_nodes;
    DirectX::XMMATRIX nav_world = DirectX::XMMatrixIdentity();

    std::unique_ptr<Level> level;
    int grid_scale;

    // Helper stuff.
    std::unique_ptr<VBGO> cursor;
    std::unique_ptr<VBGO> waypoint_indicator;

};
