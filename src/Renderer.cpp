#include "Renderer.h"
#include "Window.h"


#define SafeRelease(x) if (x) { x->Release(); x = nullptr; }

Renderer::Renderer(Window* _window)
    : window(_window)
    , swap_chain(nullptr)
    , d3d_device(nullptr)
    , device_context(nullptr)
    , viewport({ 0.f, 0.f, _window->getWidthF(), _window->getHeightF(), 0.0f, 1.0f })
    , target_view(nullptr)
    , depth_stencil_state(nullptr)
    , rasterizer_state(nullptr)
    , depth_stencil_buffer(nullptr)
    , depth_stencil_view(nullptr)
{
}


Renderer::~Renderer()
{
    SafeRelease(swap_chain);
    SafeRelease(d3d_device);
    SafeRelease(device_context);
    SafeRelease(target_view);
    SafeRelease(depth_stencil_state);
    SafeRelease(rasterizer_state);
    SafeRelease(depth_stencil_buffer);
    SafeRelease(depth_stencil_view);
}


bool Renderer::init(const DirectX::XMFLOAT4& _clear_color)
{
    if (!createDevice())
        return false;

    if (!createRenderTarget())
        return false;

    if (!createDepthStencil())
        return false;

    setClearColor(_clear_color);

    return true;
}


void Renderer::setClearColor(const DirectX::XMFLOAT4& _clear_color)
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
    device_context->OMSetRenderTargets(1, &target_view, depth_stencil_view);

    // Set clear color.
    float cc[] = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
    device_context->ClearRenderTargetView(target_view, cc);

    // Clear depth stencil.
    device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0x00);

    // Set viewport.
    device_context->RSSetViewports(1, &viewport);
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
    swap_chain_desc.OutputWindow = window->getHandle();
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.Windowed = true;

    auto hr = D3D11CreateDeviceAndSwapChain(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
        nullptr, 0, D3D11_SDK_VERSION, &swap_chain_desc,
        &swap_chain, &d3d_device, nullptr, &device_context);

    if (hr != S_OK)
        return false;

    return true;
}


bool Renderer::createRenderTarget()
{
    HRESULT hr = {};

    ID3D11Texture2D* back_buffer;
    hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);

    if (FAILED(hr))
        return false; // Failed to get back buffer.

    d3d_device->CreateRenderTargetView(back_buffer, nullptr, &target_view);

    if (FAILED(hr))
        return false; // Failed to create render target view.

    back_buffer->Release();

    return true;
}


bool Renderer::createDepthStencil()
{
    HRESULT hr = {};

    D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = { 0 };
    depth_stencil_desc.DepthEnable = true;
    depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
    depth_stencil_desc.StencilEnable = false;
    depth_stencil_desc.StencilWriteMask = 0xFFu;
    depth_stencil_desc.StencilReadMask = 0xFFu;

    depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    hr = d3d_device->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);

    if (FAILED(hr))
        return false; // Failed to create depth stencil state.

    device_context->OMSetDepthStencilState(depth_stencil_state, 1);

    return true;
}
