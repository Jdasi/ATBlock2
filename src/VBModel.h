#pragma once

#include <Windows.h>

#include <d3d11.h>

class Renderer;
struct DrawData;
struct ShaderData;
struct Vertex;

class VBModel
{
public:
    VBModel();
    ~VBModel();

    void draw(DrawData* _dd);

    ID3D11Buffer* const* getVertexBuffer() const;
    ID3D11Buffer* getIndexBuffer() const;

    ShaderData* getShader() const;
    void setShader(ShaderData* _shader);

    const D3D_PRIMITIVE_TOPOLOGY& getTopology() const;
    const UINT& getNumVertices() const;
    const UINT& getNumIndices() const;

protected:
    virtual void createMesh(Renderer* _renderer) = 0;

    void buildVB(Renderer* _renderer, const int _num_verts, Vertex* _vertices);
    void buildIB(Renderer* _renderer, DWORD* _indices);

    UINT num_vertices;
    UINT num_indices;

private:
    ID3D11Buffer* vertex_buffer;
    ID3D11Buffer* index_buffer;

    ShaderData* shader;
    D3D_PRIMITIVE_TOPOLOGY topology;

};
