#include "NFCore/NFRender.h"
#include <cassert>
#include "NFUtility/NFGlobalConfig.h"


NineForceEngine::NFRender::NFRender()
{
}


NineForceEngine::NFRender::~NFRender()
{
}


bool NineForceEngine::NFRender::Init(const HWND targetWindow)
{
    // create device
    UINT _createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUF)
    _createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL _featureLevel;

    auto _result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        _createDeviceFlags,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &mDevice,
        &_featureLevel,
        &mContext
    );

    if (FAILED(_result))
    {
        MessageBox(nullptr, L"Create D3D device failed!", L"Warning", 0);

        return false;
    }

    if (_featureLevel != D3D_FEATURE_LEVEL_11_0)
    {
        MessageBox(nullptr, L"D3D11 not supported!", L"Error", 0);

        return false;
    }
    // end


    // check 4x msaa support

    _result = mDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        4,
        &m4XMSAAQuality
    );

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Check 4XMSAA failed! please check!",
            L"Error",
            0
        );

        return false;
    }

    assert(m4XMSAAQuality > 0);

    // end

    // create swap chain
    const auto _globalIns = NFGlobalConfig::Instance();

    DXGI_SWAP_CHAIN_DESC _swapChainDesc;

    _swapChainDesc.BufferDesc.Width = _globalIns->GetResolutionWidth();

    _swapChainDesc.BufferDesc.Height = _globalIns->GetResolutionHeight();

    _swapChainDesc.BufferDesc.RefreshRate.Numerator = _globalIns->GetRefreshRate();

    _swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

    _swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

    _swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    _swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    // end

    // create swap chain
    if (_globalIns->GetUse4XMSAA())
    {
        _swapChainDesc.SampleDesc.Count = 4;

        _swapChainDesc.SampleDesc.Quality = m4XMSAAQuality - 1;
    }
    else
    {
        _swapChainDesc.SampleDesc.Count = 1;

        _swapChainDesc.SampleDesc.Quality = 0;
    }

    _swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    _swapChainDesc.BufferCount = 1;

    _swapChainDesc.OutputWindow = targetWindow;

    _swapChainDesc.Windowed = !_globalIns->GetIsFullScreen();

    _swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    _swapChainDesc.Flags = 0;

    IDXGIDevice* _dxgiDevice = nullptr;
    _result = mDevice->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(& _dxgiDevice));
    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create QueryInterface failed, please check!",
            L"Error",
            0
        );

        return false;
    }

    IDXGIAdapter* _dxgiAdapter = nullptr;
    _result = _dxgiDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(& _dxgiAdapter));
    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create DXGIAdapter failed, please check!",
            L"Error",
            0
        );

        return false;
    }

    IDXGIFactory* _dxgiFactory = nullptr;
    _result = _dxgiAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&_dxgiFactory));
    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create DXGIFactory failed, please check!",
            L"Error",
            0
        );

        return false;
    }

    _result = _dxgiFactory->CreateSwapChain(mDevice, &_swapChainDesc, &mSwapChain);
    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create SwapChain failed, please check!",
            L"Error",
            0
        );

        return false;
    }

    NFUtility::ReleaseCOM(reinterpret_cast<IUnknown**>(_dxgiFactory));

    NFUtility::ReleaseCOM(reinterpret_cast<IUnknown**>(_dxgiAdapter));

    NFUtility::ReleaseCOM(reinterpret_cast<IUnknown**>(_dxgiDevice));

    //end

    // create Render target view
    ID3D11Texture2D* _backBuffer = nullptr;

    mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&_backBuffer));

    if (_backBuffer != nullptr)
    {
        _result = mDevice->CreateRenderTargetView(_backBuffer, nullptr, &mRenderTargetView);

        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"Create render target view faild! please check!",
                L"Error",
                0
            );

            return false;
        }
    }
    else
    {
        MessageBox(
            nullptr,
            L"Get back buffer failed! please check!",
            L"Error",
            0
        );


        return false;
    }

    NFUtility::ReleaseCOM(reinterpret_cast<IUnknown**>(_backBuffer));

    // end


    // create Depth/Stencil buffer and view

    D3D11_TEXTURE2D_DESC _depthStencilDesc;

    _depthStencilDesc.Width = NFGlobalConfig::Instance()->GetResolutionWidth();

    _depthStencilDesc.Height = NFGlobalConfig::Instance()->GetResolutionHeight();

    _depthStencilDesc.MipLevels = 1;

    _depthStencilDesc.ArraySize = 1;

    _depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;

    _depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    _depthStencilDesc.CPUAccessFlags = 0; // no cpu access, it's a slow opration,should avoid

    _depthStencilDesc.MiscFlags = 0;

    ID3D11Texture2D* _depthStencilBuffer = nullptr;

    _result = mDevice->CreateTexture2D(&_depthStencilDesc, nullptr, &_depthStencilBuffer);

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create depth & stencil buffer failed, please check!",
            L"Error",
            0
        );

        return false;
    }

    _result = mDevice->CreateDepthStencilView(_depthStencilBuffer, nullptr, &mDepthStencilView);

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create depth & stencil buffer view failed! please check!",
            L"Error",
            0
        );

        return false;
    }

    NFUtility::ReleaseCOM(reinterpret_cast<IUnknown**>(_depthStencilBuffer));

    // end

    mContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);


    // set viewport
    D3D11_VIEWPORT _viewPort;

    _viewPort.TopLeftX = 0;
    _viewPort.TopLeftY = 0;
    _viewPort.Width = static_cast<float>(NFGlobalConfig::Instance()->GetResolutionWidth());
    _viewPort.Height = static_cast<float>(NFGlobalConfig::Instance()->GetResolutionHeight());
    _viewPort.MinDepth = 0.0f;
    _viewPort.MaxDepth = 1.0f;

    mContext->RSSetViewports(1, &_viewPort);

    // end

    return true;
}


void NineForceEngine::NFRender::Update(float deltaTime)
{
}


void NineForceEngine::NFRender::Clean()
{
}


void NineForceEngine::NFRender::Render()
{
}
