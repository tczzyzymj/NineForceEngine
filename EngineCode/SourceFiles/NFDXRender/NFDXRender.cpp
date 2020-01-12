#include "NFDXRender/NFDXRender.h"
#include "NFCommonInclude.h"
#include <vector>
#include "NFSetting.h"


using Microsoft::WRL::ComPtr;


NFDXRender::NFDXRender()
{
}


bool NFDXRender::Init(HWND targetHwnd)
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> _debugController;

        auto _result = D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"Create DirectX Debug failed!",
                L"Error",
                MB_OK
            );

            return false;
        }

        _debugController->EnableDebugLayer();
    }

#endif

    // Create DXGI fatory
    {
        auto _result = CreateDXGIFactory1(IID_PPV_ARGS(&mFactory));
        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"Create DXGI_Factory failed!",
                L"Error",
                MB_OK
            );

            return false;
        }
    }

    // Create Direct device
    {
        auto _result = D3D12CreateDevice(
            nullptr,
            D3D_FEATURE_LEVEL_12_0,
            IID_PPV_ARGS(&mDevice)
        );

        if (FAILED(_result))
        {
            // if failed, try get adapter from factory

            ComPtr<IDXGIAdapter> _warpAdapter = nullptr;

            _result = mFactory->EnumWarpAdapter(IID_PPV_ARGS(&_warpAdapter));

            if (FAILED(_result))
            {
                MessageBox(
                    nullptr,
                    L"Get enum wrapAdapter failed!",
                    L"Error",
                    MB_OK
                );

                return false;
            }

            _result = D3D12CreateDevice(
                _warpAdapter.Get(),
                D3D_FEATURE_LEVEL_12_0,
                IID_PPV_ARGS(&mDevice)
            );

            if (FAILED(_result))
            {
                MessageBox(
                    nullptr,
                    L"Create D3D_Device failed!",
                    L"Error",
                    MB_OK
                );

                return false;
            }
        }

        _result = mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));

        if (FAILED(_result))
        {
            MessageBox(nullptr, L"Create fence failed!", L"Error", MB_OK);

            return false;
        }
    }

    // Get descriptor size
    {
        mRtvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV
        );

        mDsvDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_DSV
        );

        mCbvSrvUavDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV
        );
    }

    // check 4x MSAA quality
    {
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS _msQualityLevels;

        _msQualityLevels.Format = mBackBufferFormat;

        _msQualityLevels.SampleCount = 4;

        _msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;

        _msQualityLevels.NumQualityLevels = 0;

        auto _result = mDevice->CheckFeatureSupport(
            D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
            &_msQualityLevels,

            sizeof(_msQualityLevels)
        );

        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"Check MSAA failed!",
                L"Error",
                MB_OK
            );

            return false;
        }

        m4sMsaaQuality = _msQualityLevels.NumQualityLevels;

        assert(m4sMsaaQuality > 0 && "Unexpected MSAA quality level.");
    }

#ifdef _DEBUG
    LogAdapters();
#endif

    if (!CreateCommandObjects())
    {
        return false;
    }

    if (!CreateSwapChain(targetHwnd))
    {
        return false;
    }

    if (!CreateRtvAndDsvDescriptionHeaps())
    {
        return false;
    }

    return true;
}


void NFDXRender::LogAdapters()
{
    UINT _i = 0;

    IDXGIAdapter* _adapter = nullptr;

    std::vector<IDXGIAdapter*> _adapterVector;

    while (mFactory->EnumAdapters(_i, &_adapter) != DXGI_ERROR_NOT_FOUND)
    {
        DXGI_ADAPTER_DESC _desc;

        _adapter->GetDesc(&_desc);

        std::wstring _text = L"***Adapter is : ";

        _text += _desc.Description;

        _text += L"\n";

        OutputDebugString(_text.c_str());

        _adapterVector.push_back(_adapter);

        ++_i;
    }

    for (UINT _j = 0; _j < _adapterVector.size(); ++_j)
    {
    }
}


bool NFDXRender::CreateCommandObjects()
{
    D3D12_COMMAND_QUEUE_DESC _queueDesc = {};

    _queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    _queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

    auto _result = mDevice->CreateCommandQueue(&_queueDesc, IID_PPV_ARGS(&mCommandQueue));

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create command failed!",
            L"Error",
            MB_OK
        );

        return false;
    }

    _result = mDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(mDirectCmdListAlloc.GetAddressOf())
    );

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create command allocator failed!",
            L"Error",
            MB_OK
        );

        return false;
    }

    _result = mDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        mDirectCmdListAlloc.Get(),
        nullptr,
        IID_PPV_ARGS(mCommandList.GetAddressOf())
    );

    if (FAILED(_result))
    {
        MessageBox(
            nullptr,
            L"Create Command List failed!",
            L"Error",
            MB_OK
        );

        return false;
    }

    mCommandList->Close();

    return true;
}


bool NFDXRender::CreateSwapChain(HWND targetHwnd)
{
    mSwapChain.Reset(); // this method is call by ComPtr;

    DXGI_SWAP_CHAIN_DESC _desc;

    _desc.BufferDesc.Width = NFSetting::GetInstance().GetClientWidth();

    _desc.BufferDesc.Height = NFSetting::GetInstance().GetClientHeight();

    _desc.BufferDesc.RefreshRate.Numerator = 60;

    _desc.BufferDesc.RefreshRate.Denominator = 1;

    _desc.BufferDesc.Format = mBackBufferFormat;

    _desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    _desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    _desc.SampleDesc.Count = NFSetting::GetInstance().GetEnable4XMSAA() ? 4 : 1;

    _desc.SampleDesc.Quality = NFSetting::GetInstance().GetEnable4XMSAA() ? (m4sMsaaQuality - 1) : 0;

    _desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    _desc.BufferCount = mSwapChainBufferCount;

    _desc.OutputWindow = targetHwnd;

    _desc.Windowed = true;

    _desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

    _desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    auto _result = mFactory->CreateSwapChain(
        mCommandQueue.Get(),
        &_desc,
        mSwapChain.GetAddressOf()
    );

    if (FAILED(_result))
    {
        MessageBox(nullptr, L"Create SwapChain failed!", L"Error", MB_OK);

        return false;
    }

    return true;
}


bool NFDXRender::CreateRtvAndDsvDescriptionHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC _rtvHeapDesc;

    _rtvHeapDesc.NumDescriptors = mSwapChainBufferCount;

    _rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

    _rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    _rtvHeapDesc.NodeMask = 0;

    auto _result = mDevice->CreateDescriptorHeap(&_rtvHeapDesc, IID_PPV_ARGS(mRtvHeap.GetAddressOf()));

    if (FAILED(_result))
    {
        MessageBox(nullptr, L"Create RTVHeap failed!", L"Error", MB_OK);

        return false;
    }

    D3D12_DESCRIPTOR_HEAP_DESC _dsvHeapDesc;

    _dsvHeapDesc.NumDescriptors = 1;

    _dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

    _dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    _dsvHeapDesc.NodeMask = 0;

    _result = mDevice->CreateDescriptorHeap(&_dsvHeapDesc, IID_PPV_ARGS(mDsvHeap.GetAddressOf()));

    if (FAILED(_result))
    {
        MessageBox(nullptr, L"Create DSVHeap failed!", L"Error", MB_OK);

        return false;
    }

    return true;
}


void NFDXRender::Render()
{
}
