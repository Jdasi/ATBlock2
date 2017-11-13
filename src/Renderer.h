#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class Window;

class Renderer
{
public:
    Renderer(Window* _window);
    ~Renderer();

    bool init(const DirectX::XMFLOAT4& _clear_color);
    void setClearColor(const DirectX::XMFLOAT4& _clear_color);

    ID3D11Device* getDevice() const;
    ID3D11DeviceContext* getDeviceContext() const;

    void beginFrame();
    void endFrame();

private:
    bool createDevice();
    bool createRenderTarget();
    bool createDepthStencil();

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
    ID3D11RasterizerState* raster_state;

};
