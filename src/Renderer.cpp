#include "Renderer.h"
#include "Window.h"


Renderer::Renderer(Window& _window)
    : window(_window)
    , swap_chain(nullptr)
    , d3d_device(nullptr)
    , device_context(nullptr)
    , target_view(nullptr)
{
}


bool Renderer::init(const JMath::Vector4& _clear_color)
{
    if (!createDevice())
        return false;

    if (!createRenderTarget())
        return false;

    setClearColor(_clear_color);

    return true;
}


void Renderer::setClearColor(const JMath::Vector4& _clear_color)
{
    clear_color = _clear_color;
}


ID3D11Device* Renderer::getDevice() const
{
    return d3d_device;
}


ID3D11DeviceContext* Renderer::getDeviceContext() const
{
    return device_context;
}


void Renderer::beginFrame()
{
    // Bind render target.
    device_context->OMSetRenderTargets(1, &target_view, nullptr); // last parameter is depth.

    // Set viewport.
    auto viewport = CD3D11_VIEWPORT(0.0f, 0.0f, window.getWidth(), window.getHeight());
    device_context->RSSetViewports(1, &viewport);

    // Set clear color.
    float cc[] = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
    device_context->ClearRenderTargetView(target_view, cc);
}

void Renderer::endFrame()
{
    swap_chain->Present(1, 0);
}


bool Renderer::createDevice()
{
    DXGI_SWAP_CHAIN_DESC swap_chain_desc = { 0 };
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.OutputWindow = window.getHandle();
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = true;

    auto result = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
        &swap_chain, &d3d_device, nullptr, &device_context);

    if (result != S_OK)
        return false;

    return true;
}


bool Renderer::createRenderTarget()
{
    ID3D11Texture2D* back_buffer;
    swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);
    d3d_device->CreateRenderTargetView(back_buffer, nullptr, &target_view);
    back_buffer->Release();

    return true;
}
