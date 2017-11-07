#pragma once

#include <d3d11.h>

#include "JMath.h"

class Window;

class Renderer
{
public:
    Renderer(Window& _window);

    bool init(const JMath::Vector4& _clear_color);
    void setClearColor(const JMath::Vector4& _clear_color);

    ID3D11Device* getDevice() const;
    ID3D11DeviceContext* getDeviceContext() const;

    void beginFrame();
    void endFrame();

private:
    bool createDevice();
    bool createRenderTarget();

    Window& window;
    JMath::Vector4 clear_color;

    // Device stuff.
    IDXGISwapChain* swap_chain;
    ID3D11Device* d3d_device;
    ID3D11DeviceContext* device_context;

    // Render target.
    ID3D11RenderTargetView* target_view;

};
