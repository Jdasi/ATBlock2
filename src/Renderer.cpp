#include <fstream>
#include <iostream>

#include "Renderer.h"
#include "Window.h"
#include "DXUtil.h"


Renderer::Renderer(Window* _window)
    : window(_window)
    , swap_chain(nullptr)
    , d3d_device(nullptr)
    , device_context(nullptr)
    , viewport({ 0.0f, 0.0f, _window->getWidthF(), _window->getHeightF(), 0.0f, 1.0f })
    , target_view(nullptr)
    , depth_stencil_buffer(nullptr)
    , depth_stencil_view(nullptr)
    , raster_state_solid(nullptr)
    , raster_state_wire(nullptr)
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
    SAFE_RELEASE(raster_state_solid);
    SAFE_RELEASE(raster_state_wire);
}


bool Renderer::init(const DirectX::XMFLOAT4& _clear_color)
{
    if (!createDevice())
        return false;

    if (!createRenderTarget())
        return false;

    if (!createDepthStencil())
        return false;

    if (!createShaders())
        return false;

    setClearColor(_clear_color);

    // Bind render target.
    device_context->OMSetRenderTargets(1, &target_view, depth_stencil_view);

    return true;
}


void Renderer::setClearColor(const DirectX::XMFLOAT4& _clear_color)
{
    clear_color = _clear_color;
}


void Renderer::setRenderStyle(const RenderStyle& _render_style)
{
    switch (_render_style)
    {
        case SOLID:
        {
            device_context->RSSetState(raster_state_solid);
        } break;

        case WIREFRAME:
        {
            device_context->RSSetState(raster_state_wire);
        } break;
    }
}


ID3D11Device* Renderer::getDevice() const
{
    return d3d_device;
}


ID3D11DeviceContext* Renderer::getDeviceContext() const
{
    return device_context;
}


ShaderData* Renderer::getShaderData(const ShaderType& _type)
{
    return shaders[_type].get();
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

    auto hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, NULL, NULL,
        D3D11_SDK_VERSION, &swap_chain_desc, &swap_chain, &d3d_device, NULL, &device_context);

    if (FAILED(hr))
    {
        std::cout << "Failed to create Device and Swap Chain" << std::endl;
        return false;
    }

    return true;
}


bool Renderer::createRenderTarget()
{
    HRESULT hr = {};

    ID3D11Texture2D* back_buffer;
    hr = swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&back_buffer);

    if (FAILED(hr))
    {
        std::cout << "Failed to get Back Buffer" << std::endl;
        return false;
    }

    hr = d3d_device->CreateRenderTargetView(back_buffer, NULL, &target_view);
    back_buffer->Release();

    if (FAILED(hr))
    {
        std::cout << "Failed to create Render Target View." << std::endl;
        return false;
    }

    return true;
}


bool Renderer::createDepthStencil()
{
    HRESULT hr;

    D3D11_TEXTURE2D_DESC depth_stencil_desc;

    depth_stencil_desc.Width = window->getWidth();
    depth_stencil_desc.Height = window->getHeight();
    depth_stencil_desc.MipLevels = 1;
    depth_stencil_desc.ArraySize = 1;
    depth_stencil_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depth_stencil_desc.SampleDesc.Count = 1;
    depth_stencil_desc.SampleDesc.Quality = 0;
    depth_stencil_desc.Usage = D3D11_USAGE_DEFAULT;
    depth_stencil_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depth_stencil_desc.CPUAccessFlags = 0;
    depth_stencil_desc.MiscFlags = 0;

    // Create the Depth/Stencil View.
    hr = d3d_device->CreateTexture2D(&depth_stencil_desc, NULL, &depth_stencil_buffer);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Depth Stencil Texture." << std::endl;
        return false;
    }

    hr = d3d_device->CreateDepthStencilView(depth_stencil_buffer, NULL, &depth_stencil_view);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Depth Stencil View." << std::endl;
        return false;
    }

    // Set up Raster State.
    D3D11_RASTERIZER_DESC raster_desc;

    raster_desc.AntialiasedLineEnable = false;
    raster_desc.CullMode = D3D11_CULL_BACK;
    raster_desc.DepthBias = 0;
    raster_desc.DepthBiasClamp = 0.0f;
    raster_desc.DepthClipEnable = true;
    raster_desc.FillMode = D3D11_FILL_SOLID;
    raster_desc.FrontCounterClockwise = false;
    raster_desc.MultisampleEnable = false;
    raster_desc.ScissorEnable = false;
    raster_desc.SlopeScaledDepthBias = 0.0f;

    // Create Default Raster State.
    hr = d3d_device->CreateRasterizerState(&raster_desc, &raster_state_solid);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Rasteriser State." << std::endl;
        return false;
    }

    raster_desc.FillMode = D3D11_FILL_WIREFRAME;

    // Create Wireframe Raster State.
    hr = d3d_device->CreateRasterizerState(&raster_desc, &raster_state_wire);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Rasteriser State." << std::endl;
        return false;
    }

    // Set default raster state.
    device_context->RSSetState(raster_state_solid);

    return true;
}


bool Renderer::createShaders()
{
    HRESULT hr;
    auto shader = std::make_unique<ShaderData>();

    using namespace std;

    // Create shaders.
    ifstream vs_file("Resources/Shaders/VertexShader.cso", ios::binary);
    ifstream ps_file("Resources/Shaders/PixelShader.cso", ios::binary);

    vector<char> vs_data = { istreambuf_iterator<char>(vs_file), istreambuf_iterator<char>() };
    vector<char> ps_data = { istreambuf_iterator<char>(ps_file), istreambuf_iterator<char>() };

    hr = d3d_device->CreateVertexShader(vs_data.data(), vs_data.size(), NULL, &shader->vertex_shader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Vertex Shader" << std::endl;
        return false;
    }

    hr = d3d_device->CreatePixelShader(ps_data.data(), ps_data.size(), NULL, &shader->pixel_shader);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Pixel Shader" << std::endl;
        return false;
    }

    // Create input layouts.
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Data from the Vertex Buffer.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },

        // Data from the Instance buffer.
        { "INSTANCEPOS", 0, DXGI_FORMAT_R32G32B32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
    };

    hr = d3d_device->CreateInputLayout(layout, 2, vs_data.data(), vs_data.size(), &shader->input_layout);
    if (FAILED(hr))
    {
        std::cout << "Failed to create Input Layout" << std::endl;
        return false;
    }

    shaders.push_back(std::move(shader));

    return true;
}
