#pragma once

#include <vector>

#include "Renderer.h"
#include "Vertex.h"
#include "VBModel.h"
#include "ShaderData.h"
#include "SwarmAgent.h"

struct GameData;
struct DrawData;

class SwarmManager
{
public:
    SwarmManager(Renderer* _renderer, VBModel* _agent_model, const int _num_agents);
    ~SwarmManager() = default;

    void tick(GameData* _gd);
    void draw(DrawData* _dd);

private:
    void createConstantBuffers(Renderer* _renderer);
    void configureInstanceBuffer();
    void updateInstanceBuffer();

    Renderer* renderer;

    ID3D11Buffer* cb_per_instance_buffer;
    ID3D11Buffer* agent_instance_buff;

    ID3D11Buffer* cb_gpu;
    CBPerObject* cb_cpu;

    D3D11_BUFFER_DESC instance_buffer_desc;
    D3D11_SUBRESOURCE_DATA inst_resource_data;

    std::vector<SwarmAgent> agents;
    DirectX::XMMATRIX agent_world = DirectX::XMMatrixIdentity();

    VBModel* agent_model;
    int num_agents;

};
