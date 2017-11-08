#pragma once

#include "Renderer.h"

class Square
{
public:
    Square(Renderer* _renderer);
    ~Square();

    void draw(Renderer* _renderer);

private:
    void createMesh(Renderer* _renderer);
    void createShaders(Renderer* _renderer);

    ID3D11Buffer* vertex_buffer;
    ID3D11Buffer* index_buffer;
    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11InputLayout* input_layout;

};
