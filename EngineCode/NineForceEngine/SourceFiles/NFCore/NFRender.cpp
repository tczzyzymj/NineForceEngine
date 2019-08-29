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
    D3D_FEATURE_LEVEL _featureLevel;

    auto _result = D3D11CreateDevice(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
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


    // check 4x msaa support

    _result = mDevice->CheckMultisampleQualityLevels(
        DXGI_FORMAT_R8G8B8A8_UNORM,
        4,
        &m4XMSAAQuality
    );

    assert(m4XMSAAQuality > 0);

    // end

    // create swap chain
    const auto _globalIns = NFGlobalConfig::Instance();

    DXGI_SWAP_CHAIN_DESC _swapChainDesc;

    _swapChainDesc.BufferDesc.Width = _globalIns->GetScreenWidth();

    _swapChainDesc.BufferDesc.Height = _globalIns->GetScreenHeight();

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

    ReleaseCom(reinterpret_cast<IUnknown**>(_dxgiFactory));

    ReleaseCom(reinterpret_cast<IUnknown**>(_dxgiAdapter));

    ReleaseCom(reinterpret_cast<IUnknown**>(_dxgiDevice));

    //end

    return true;
}


void NineForceEngine::NFRender::Update(float deltaTime)
{
}


void NineForceEngine::NFRender::Clean()
{
}


void NineForceEngine::NFRender::ReleaseCom(IUnknown** targetPtr)
{
    if (targetPtr == nullptr || *targetPtr == nullptr)
    {
        return;
    }

    (*targetPtr)->Release();

    *targetPtr = nullptr;
}


void NineForceEngine::NFRender::Render()
{
}
