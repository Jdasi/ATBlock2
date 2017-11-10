#pragma once

#include "GameObject.h"

class VBGO : public GameObject
{
public:
    VBGO(Renderer* _renderer);
    virtual ~VBGO();

    virtual void tick(GameData* _gd) override;
    virtual void draw(DrawData* _dd) override;

protected:
    virtual void createMesh(Renderer* _renderer);

    void buildVB(Renderer* _renderer, const int _num_verts, Vertex* _vertices);
    void buildIB(Renderer* _renderer, DWORD* _indices);

    ID3D11Buffer* vertex_buffer;
    ID3D11Buffer* index_buffer;

    ID3D11Buffer* cb_gpu;
    ConstantBuffer* cb_cpu;
    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11InputLayout* input_layout;

    D3D_PRIMITIVE_TOPOLOGY topology;

    UINT num_vertices;
    UINT num_indices;

private:
    void init(Renderer* _renderer);

    void createShaders(Renderer* _renderer);
    void createConstantBuffers(Renderer* _renderer);

};
