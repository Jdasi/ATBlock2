#pragma once

#include <vector>
#include <memory>

#include <d3d11.h>
#include <DirectXMath.h>

#include "ShaderData.h"

class Window;

/* Encapuslates the DirectX11 rendering system and supporting information.
 * Graphical objects can request information from the Renderer to allow
 * the object to be drawn correctly.
 */
class Renderer
{
public:
    enum RenderStyle : int
    {
        SOLID,
        WIREFRAME
    };

    enum ShaderType : int
    {
        DEFAULT,
        INSTANCED
    };

    Renderer(Window* _window);
    ~Renderer();

    bool init(const DirectX::XMFLOAT4& _clear_color);
    void setClearColor(const DirectX::XMFLOAT4& _clear_color);
    void setRenderStyle(const RenderStyle& _render_style);

    ID3D11Device* getDevice() const;
    ID3D11DeviceContext* getDeviceContext() const;

    ShaderData* getShaderData(const ShaderType& _type);

    void beginFrame();
    void endFrame();

private:
    bool createDevice();
    bool createRenderTarget();
    bool createDepthStencil();
    bool createShaders();

    bool createDefaultShader(std::vector<char>& _vs_data, std::vector<char>& _ps_data);
    bool createInstancedShader(std::vector<char>& _vs_data, std::vector<char>& _ps_data);

    bool compileShaderFromHLSL(const std::string& _name, ShaderData* _shader,
        std::vector<char>& _vs_data, std::vector<char>& _ps_data);

    Window* window;
    DirectX::XMFLOAT4 clear_color;

    // Device stuff.
    IDXGISwapChain* swap_chain;
    ID3D11Device* d3d_device;
    ID3D11DeviceContext* device_context;

    // Viewport.
    D3D11_VIEWPORT viewport;

    // Render target.
    ID3D11RenderTargetView* target_view;

    // Stencil stuff.
    ID3D11Texture2D* depth_stencil_buffer;
    ID3D11DepthStencilView* depth_stencil_view;

    // Raster stuff.
    ID3D11RasterizerState* raster_state_solid;
    ID3D11RasterizerState* raster_state_wire;

    // Shaders.
    std::vector<std::unique_ptr<ShaderData>> shaders;

};
