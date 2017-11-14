#pragma once

#include <Windows.h>

#include <d3d11.h>

#include "Vertex.h"
#include "ShaderData.h"

class Renderer;
struct DrawData;

class VBModel final
{
public:
    VBModel(Renderer* _renderer);
    ~VBModel();

    void draw(DrawData* _dd);

    ID3D11Buffer* const* getVertexBuffer() const;
    ID3D11Buffer* getIndexBuffer() const;
    ShaderData* getShader() const;
    const D3D_PRIMITIVE_TOPOLOGY& getTopology() const;
    const UINT& getNumVertices() const;
    const UINT& getNumIndices() const;

private:
    void createMesh(Renderer* _renderer);

    void buildVB(Renderer* _renderer, const int _num_verts, Vertex* _vertices);
    void buildIB(Renderer* _renderer, DWORD* _indices);

    ID3D11Buffer* vertex_buffer;
    ID3D11Buffer* index_buffer;

    ShaderData* shader;
    D3D_PRIMITIVE_TOPOLOGY topology;

    UINT num_vertices;
    UINT num_indices;

};
