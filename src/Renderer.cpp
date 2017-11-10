#include "Renderer.h"
#include "Window.h"
#include "DXUtil.h"


Renderer::Renderer(Window* _window)
    : window(_window)
    , swap_chain(nullptr)
    , d3d_device(nullptr)
    , device_context(nullptr)
    , viewport({ 0.f, 0.f, _window->getWidthF(), _window->getHeightF(), 0.0f, 1.0f })
    , target_view(nullptr)
    , depth_stencil_buffer(nullptr)
    , depth_stencil_view(nullptr)
{
}


Renderer::~Renderer()
{
    SAFE_RELEASE(swap_chain);
    SAFE_RELEASE(d3d_device);
    SAFE_RELEASE(device_context);
    SAFE_RELEASE(target_view);
    SAFE_RELEASE(depth_stencil_buffer);
    SAFE_RELEASE(depth_stencil_view);
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
    // Set clear color.
    float cc[] = { clear_color.x, clear_color.y, clear_color.z, clear_color.w };
    device_context->ClearRenderTargetView(target_view, cc);

    // Clear depth stencil.
    device_context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    // Set viewport.
    device_context->RSSetViewports(1, &viewport);
}


void Renderer::endFrame()
{
    swap_chain->Present(1, 0);
}


bool Renderer::createDevice()
{
    DXGI_MODE_DESC buffer_desc;
    ZeroMemory(&buffer_desc, sizeof(DXGI_MODE_DESC));

    buffer_desc.Width = window->getWidth();
    buffer_desc.Height = window->getHeight();
    buffer_desc.RefreshRate.Numerator = 60;
    buffer_desc.RefreshRate.Denominator = 1;
    buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SWAP_CHAIN_DESC swap_chain_desc;
    ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));

    swap_chain_desc.BufferDesc = buffer_desc;
    swap_chain_desc.SampleDesc.Count = 1;
    swap_chain_desc.SampleDesc.Quality = 0;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.BufferCount = 1;
    swap_chain_desc.OutputWindow = window->getHandle();
    swap_chain_desc.Windowed = true;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    auto hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL,
        D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &d3d_device, NULL, &device_context);

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

    // Bind render target.
    device_context->OMSetRenderTargets(1, &target_view, depth_stencil_view);

    return true;
}


bool Renderer::createDepthStencil()
{
    HRESULT hr = {};

    D3D11_TEXTURE2D_DESC depthStencilDesc;

    depthStencilDesc.Width = window->getWidth();
    depthStencilDesc.Height = window->getHeight();
    depthStencilDesc.MipLevels = 1;
    depthStencilDesc.ArraySize = 1;
    depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
    depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilDesc.CPUAccessFlags = 0;
    depthStencilDesc.MiscFlags = 0;

    //Create the Depth/Stencil View
    d3d_device->CreateTexture2D(&depthStencilDesc, NULL, &depth_stencil_buffer);
    d3d_device->CreateDepthStencilView(depth_stencil_buffer, NULL, &depth_stencil_view);

    return true;
}
