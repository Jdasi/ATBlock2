#pragma once

#include "Renderer.h"

class Triangle
{
public:
    Triangle(Renderer& _renderer);
    ~Triangle();

    void draw(Renderer& _renderer);

private:
    void createMesh(Renderer& _renderer);
    void createShaders(Renderer& _renderer);

    ID3D11Buffer* vertex_buffer;
    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11InputLayout* input_layout;

};
