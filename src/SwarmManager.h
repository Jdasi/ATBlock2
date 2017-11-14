#pragma once

#include <vector>

#include "Renderer.h"
#include "Vertex.h"
#include "VBModel.h"
#include "ShaderData.h"

struct GameData;
struct DrawData;

struct InstanceData
{
    DirectX::XMFLOAT3 pos;
};

class SwarmManager
{
public:
    SwarmManager(Renderer* _renderer, VBModel* _agent_model, const int _num_agents);
    ~SwarmManager() = default;

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
    Renderer* renderer;

    std::vector<InstanceData> agents;

    ID3D11Buffer* cb_perinst_buff;
    ID3D11Buffer* agent_instance_buff;
    ID3D11Buffer* cb_gpu;
    CBPerObject* cb_cpu;

    DirectX::XMMATRIX agent_world = DirectX::XMMatrixIdentity();

    VBModel* agent_model;
    int num_agents;

};
