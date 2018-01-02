#pragma once

#include <string>

#include <d3d11.h>

#include "DXUtil.h"

/* Contains information required by graphical objects for the purposes of rendering to screen.
 * DirectX11 data structures are automatically released when this object is destroyed.
 */
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

    std::string name;
    ID3D11VertexShader* vertex_shader;
    ID3D11PixelShader* pixel_shader;
    ID3D11InputLayout* input_layout;
};
