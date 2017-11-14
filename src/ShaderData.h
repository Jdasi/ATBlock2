#pragma once

#include <d3d11.h>

#include "DXUtil.h"

struct ShaderData
{
    ShaderData()
        : vertex_shader(nullptr)
        , pixel_shader(nullptr)
        , input_layout(nullptr)
    {
    }

    ~ShaderData()
    {
        SAFE_RELEASE(vertex_shader);
        SAFE_RELEASE(pixel_shader);
        SAFE_RELEASE(input_layout);
    }

    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11InputLayout* input_layout;
};
