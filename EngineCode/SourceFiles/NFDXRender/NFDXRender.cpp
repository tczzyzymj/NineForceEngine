#include "NFDXRender/NFDXRender.h"
#include "NFCommonInclude.h"
#include <vector>
#include "NFSetting.h"
#include "DirectXColors.h"
#include "DirectXMath.h"


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

        m4xMsaaQuality = _msQualityLevels.NumQualityLevels;

        assert(m4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
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

    if (!BuildRtvAndDsvDescriptionHeaps())
    {
        return false;
    }

    ThrowIfFailed(mCommandList->Reset(mCommandAllocator.Get(), nullptr));

    // build constant buffer descriptor heaps
    ThrowIfFailed(!BuildCbvDescriptionHeaps());

    // build constant buffer
    ThrowIfFailed(!BuildConstantBuffer());

    ThrowIfFailed(!BuildRootSignature());

    ThrowIfFailed(!BuildShadersAndInputLayout());

    ThrowIfFailed(!BuildBoxGeometry());

    ThrowIfFailed(!BuildPipelineState());

    ThrowIfFailed(mCommandList->Close());

    ID3D12CommandList* _cmdArray[] = {mCommandList.Get()};

    mCommandQueue->ExecuteCommandLists(_countof(_cmdArray), _cmdArray);

    FlushCommandQueue();

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
        IID_PPV_ARGS(mCommandAllocator.GetAddressOf())
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
        mCommandAllocator.Get(),
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

    _desc.BufferDesc.Width = NFSetting::Ins().GetClientWidth();

    _desc.BufferDesc.Height = NFSetting::Ins().GetClientHeight();

    _desc.BufferDesc.RefreshRate.Numerator = 60;

    _desc.BufferDesc.RefreshRate.Denominator = 1;

    _desc.BufferDesc.Format = mBackBufferFormat;

    _desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

    _desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    _desc.SampleDesc.Count = NFSetting::Ins().GetEnable4xMsaa() ? 4 : 1;

    _desc.SampleDesc.Quality = NFSetting::Ins().GetEnable4xMsaa() ? (m4xMsaaQuality - 1) : 0;

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


bool NFDXRender::BuildRtvAndDsvDescriptionHeaps()
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


bool NFDXRender::BuildCbvDescriptionHeaps()
{
    D3D12_DESCRIPTOR_HEAP_DESC _desc = {};

    ZeroMemory(&_desc, sizeof(_desc));

    _desc.NumDescriptors = 1;
    _desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    _desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    _desc.NodeMask = 0;

    ThrowIfFailed(mDevice->CreateDescriptorHeap(&_desc, IID_PPV_ARGS(mCbvHeap.GetAddressOf())));

    return true;
}


bool NFDXRender::BuildConstantBuffer()
{
    mObjCB = std::make_unique<NFUploadBuffer<ObjectConstants>>(mDevice.Get(), 1, true);

    D3D12_GPU_VIRTUAL_ADDRESS _cbAdress = mObjCB->Resource()->GetGPUVirtualAddress();

    int _boxBufferIndex = 0;

    UINT _tempSize = NFUtility::Ins().CalcConstantBufferByteSize(sizeof(ObjectConstants));

    _cbAdress += _boxBufferIndex * _tempSize;

    D3D12_CONSTANT_BUFFER_VIEW_DESC _cbvDesc;
    _cbvDesc.BufferLocation = _cbAdress;
    _cbvDesc.SizeInBytes = _tempSize;


    mDevice->CreateConstantBufferView(
        &_cbvDesc,
        mCbvHeap->GetCPUDescriptorHandleForHeapStart()
    );

    return true;
}


bool NFDXRender::BuildRootSignature()
{
    D3D12_ROOT_PARAMETER _rootParams[1];

    D3D12_DESCRIPTOR_RANGE _cbvTable;
    ZeroMemory(&_cbvTable, sizeof(_cbvTable));

    _cbvTable.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
    _cbvTable.NumDescriptors = 1;
    _cbvTable.BaseShaderRegister = 0;
    _cbvTable.RegisterSpace = 0;
    _cbvTable.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

    _rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
    _rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
    _rootParams[0].DescriptorTable.NumDescriptorRanges = 1;
    _rootParams[0].DescriptorTable.pDescriptorRanges = &_cbvTable;

    D3D12_ROOT_SIGNATURE_DESC _rootSignDesc = {};
    ZeroMemory(&_rootSignDesc, sizeof(_rootSignDesc));

    _rootSignDesc.NumParameters = 1;
    _rootSignDesc.pParameters = _rootParams;
    _rootSignDesc.NumStaticSamplers = 0;
    _rootSignDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
    _rootSignDesc.pStaticSamplers = nullptr;

    ComPtr<ID3DBlob> _serializedRootSign = nullptr;
    ComPtr<ID3DBlob> _errorBlob = nullptr;

    HRESULT _tempResult = D3D12SerializeRootSignature(
        &_rootSignDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        _serializedRootSign.GetAddressOf(),
        _errorBlob.GetAddressOf()
    );

    if (_errorBlob != nullptr)
    {
#ifdef UNICODE
        OutputDebugStringW(static_cast<wchar_t*>(_errorBlob->GetBufferPointer()));
#else
        OutputDebugStringA(static_cast<char*>(_errorBlob->GetBufferPointer()));
#endif
    }

    ThrowIfFailed(_tempResult);

    ThrowIfFailed(
        mDevice->CreateRootSignature(
            0,
            _serializedRootSign->GetBufferPointer(),
            _serializedRootSign->GetBufferSize(),
            IID_PPV_ARGS(mRootSignature.GetAddressOf())
        )
    );

    return true;
}


bool NFDXRender::BuildShadersAndInputLayout()
{
    mVertexShader = mShaderManager->CompileShader(
        L"ShaderFiles\\SimpleColor.hlsl",
        "VS",
        "vs_5_0",
        nullptr
    );

    mPixShader = mShaderManager->CompileShader(
        L"ShaderFiles\\SimpleColor.hlsl",
        "PS",
        "ps_5_0",
        nullptr
    );

    mInputLayout =
    {
        {
            "POSITION",
            0,
            DXGI_FORMAT_R32G32B32_FLOAT,
            0,
            0,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            0
        },
        {
            "COLOR",
            0,
            DXGI_FORMAT_R32G32B32A32_FLOAT,
            0,
            12,
            D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,
            0
        }
    };

    return true;
}


bool NFDXRender::BuildBoxGeometry()
{
    return true;
}


bool NFDXRender::InitShader()
{
    mShaderManager = new NFShaderManager();

    if (mShaderManager == nullptr)
    {
        return false;
    }

    return mShaderManager->Init();
}


bool NFDXRender::BuildPipelineState()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC _desc;

    ZeroMemory(&_desc, sizeof(_desc));

    _desc.InputLayout = {mInputLayout.data(), static_cast<UINT>(mInputLayout.size())};
    _desc.pRootSignature = mRootSignature.Get();
    _desc.VS =
    {
        reinterpret_cast<BYTE*>(mVertexShader->GetBufferPointer()),
        mVertexShader->GetBufferSize()
    };
    _desc.PS =
    {
        reinterpret_cast<BYTE*>(mPixShader->GetBufferPointer()),
        mPixShader->GetBufferSize()
    };

    // set rasterizer desc
    {
        D3D12_RASTERIZER_DESC _rasterizerDesc = {};
        ZeroMemory(&_rasterizerDesc, sizeof(_rasterizerDesc));
        _rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        _rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        _rasterizerDesc.FrontCounterClockwise = FALSE;
        _rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        _rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        _rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        _rasterizerDesc.DepthClipEnable = TRUE;
        _rasterizerDesc.MultisampleEnable = FALSE;
        _rasterizerDesc.AntialiasedLineEnable = FALSE;
        _rasterizerDesc.ForcedSampleCount = 0;
        _rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
        _desc.RasterizerState = _rasterizerDesc;
    }

    // set blend desc
    {
        D3D12_BLEND_DESC _blendDesc = {};
        ZeroMemory(&_blendDesc, sizeof(_blendDesc));
        _blendDesc.AlphaToCoverageEnable = FALSE;
        _blendDesc.IndependentBlendEnable = FALSE;
        const D3D12_RENDER_TARGET_BLEND_DESC _defalutRenderTargetBlendDesc =
        {
            FALSE,
            FALSE,
            D3D12_BLEND_ONE,
            D3D12_BLEND_ZERO,
            D3D12_BLEND_OP_ADD,
            D3D12_BLEND_ONE,
            D3D12_BLEND_ZERO,
            D3D12_BLEND_OP_ADD,
            D3D12_LOGIC_OP_NOOP,
            D3D12_COLOR_WRITE_ENABLE_ALL,
        };

        for (UINT _i = 0; _i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; ++_i)
        {
            _blendDesc.RenderTarget[_i] = _defalutRenderTargetBlendDesc;
        }
        _desc.BlendState = _blendDesc;
    }

    // set depth stencil desc
    {
        D3D12_DEPTH_STENCIL_DESC _depthStencilDesc = {};
        ZeroMemory(&_depthStencilDesc, sizeof(_depthStencilDesc));
        _depthStencilDesc.DepthEnable = TRUE;
        _depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        _depthStencilDesc.StencilEnable = FALSE;
        _depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
        _depthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
        _depthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
        const D3D12_DEPTH_STENCILOP_DESC _defaultStencilOptionDesc =
        {
            D3D12_STENCIL_OP_KEEP,
            D3D12_STENCIL_OP_KEEP,
            D3D12_STENCIL_OP_KEEP,
            D3D12_COMPARISON_FUNC_ALWAYS
        };
        _depthStencilDesc.FrontFace = _defaultStencilOptionDesc;
        _depthStencilDesc.BackFace = _defaultStencilOptionDesc;

        _desc.DepthStencilState = _depthStencilDesc;
    }

    _desc.SampleMask = UINT_MAX;
    _desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    _desc.NumRenderTargets = 1;
    _desc.RTVFormats[0] = mBackBufferFormat;
    _desc.SampleDesc.Count = NFSetting::Ins().GetEnable4xMsaa() ? 4 : 1;
    _desc.SampleDesc.Quality = NFSetting::Ins().GetEnable4xMsaa() ? (m4xMsaaQuality - 1) : 0;
    _desc.DSVFormat = mDepthStencilFormat;

    ThrowIfFailed(mDevice->CreateGraphicsPipelineState(&_desc, IID_PPV_ARGS(mPipeLineState.GetAddressOf())));

    return true;
}


bool NFDXRender::Render()
{
    ThrowIfFailed(mCommandAllocator->Reset());

    ThrowIfFailed(mCommandList->Reset( mCommandAllocator.Get(), nullptr ));

    {
        D3D12_RESOURCE_BARRIER _desc;

        _desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        _desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        _desc.Transition.pResource = CurrentBackBuffer();
        _desc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
        _desc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
        _desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        mCommandList->ResourceBarrier(1, &_desc);
    }

    mCommandList->RSSetViewports(1, &mScreenViewport);

    mCommandList->RSSetScissorRects(1, &mScissorRect);

    mCommandList->ClearRenderTargetView(
        CurrentBackBufferView(),
        DirectX::Colors::Gray,
        0,
        nullptr
    );

    mCommandList->ClearDepthStencilView(
        DepthStencilView(),
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,
        1.0f,
        0,
        0,
        nullptr
    );

    mCommandList->OMSetRenderTargets(
        1,
        &CurrentBackBufferView(),
        true,
        &DepthStencilView()
    );

    {
        D3D12_RESOURCE_BARRIER _desc;

        _desc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        _desc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        _desc.Transition.pResource = CurrentBackBuffer();
        _desc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
        _desc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
        _desc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

        mCommandList->ResourceBarrier(1, &_desc);
    }

    ThrowIfFailed(mCommandList->Close());

    ID3D12CommandList* _cmdsLists[] = {mCommandList.Get()};

    mCommandQueue->ExecuteCommandLists(_countof(_cmdsLists), _cmdsLists);

    ThrowIfFailed(mSwapChain->Present(0, 0));

    mCurrentBackBuffer = (mCurrentBackBuffer + 1) % mSwapChainBufferCount;

    FlushCommandQueue();

    return true;
}


bool NFDXRender::OnResize()
{
    assert(mDevice);

    assert(mSwapChain);

    assert(mCommandAllocator);

    FlushCommandQueue();

    ThrowIfFailed(
        mCommandList->Reset(mCommandAllocator.Get(), nullptr)
    );

    for (int i = 0; i < mSwapChainBufferCount; ++i)
    {
        mSwapChainBuffer[i].Reset();
    }

    mDepthStencilBuffer.Reset();

    ThrowIfFailed(
        mSwapChain->ResizeBuffers(
            mSwapChainBufferCount,
            NFSetting::Ins().GetClientWidth(),
            NFSetting::Ins().GetClientHeight(),
            mBackBufferFormat,
            DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
        )
    );

    mCurrentBackBuffer = 0;

    auto _rtvHeap = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

    for (UINT _i = 0; _i < mSwapChainBufferCount; ++_i)
    {
        ThrowIfFailed(mSwapChain->GetBuffer(_i, IID_PPV_ARGS(&mSwapChainBuffer[_i])));

        mDevice->CreateRenderTargetView(
            mSwapChainBuffer[_i].Get(),
            nullptr,
            _rtvHeap
        );

        _rtvHeap.ptr += (mRtvDescriptorSize);
    }

    D3D12_RESOURCE_DESC _depthStencilDesc = {};

    _depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    _depthStencilDesc.Alignment = 0;
    _depthStencilDesc.Width = NFSetting::Ins().GetClientWidth();
    _depthStencilDesc.Height = NFSetting::Ins().GetClientHeight();
    _depthStencilDesc.DepthOrArraySize = 1;
    _depthStencilDesc.MipLevels = 1;
    _depthStencilDesc.Format = mDepthStencilFormat;
    _depthStencilDesc.SampleDesc.Count = NFSetting::Ins().GetEnable4xMsaa() ? 4 : 1;
    _depthStencilDesc.SampleDesc.Quality = NFSetting::Ins().GetEnable4xMsaa() ? (m4xMsaaQuality - 1) : 0;
    _depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    _depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE _optionClear;

    _optionClear.Format = mDepthStencilFormat;
    _optionClear.DepthStencil.Depth = 1.0f;
    _optionClear.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES _heapProperties = {};

    _heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;
    _heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    _heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    _heapProperties.CreationNodeMask = 1;
    _heapProperties.VisibleNodeMask = 1;

    ThrowIfFailed(
        mDevice->CreateCommittedResource(
            &_heapProperties,
            D3D12_HEAP_FLAG_NONE,
            &_depthStencilDesc,
            D3D12_RESOURCE_STATE_COMMON,
            &_optionClear,
            IID_PPV_ARGS(mDepthStencilBuffer.GetAddressOf())
        )
    );

    mDevice->CreateDepthStencilView(
        mDepthStencilBuffer.Get(),
        nullptr,
        mDsvHeap->GetCPUDescriptorHandleForHeapStart()
    );

    D3D12_RESOURCE_BARRIER _barrierDesc = {};
    ZeroMemory(&_barrierDesc, sizeof(_barrierDesc));
    _barrierDesc.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    _barrierDesc.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    _barrierDesc.Transition.pResource = mDepthStencilBuffer.Get();
    _barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    _barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_DEPTH_WRITE;
    _barrierDesc.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

    mCommandList->ResourceBarrier(
        1,
        &_barrierDesc
    );

    ThrowIfFailed(mCommandList->Close());

    ID3D12CommandList* _cmdList[] = {mCommandList.Get()};

    mCommandQueue->ExecuteCommandLists(
        _countof(_cmdList),
        _cmdList
    );

    FlushCommandQueue();

    mScreenViewport.TopLeftX = 0;
    mScreenViewport.TopLeftY = 0;
    mScreenViewport.Width = static_cast<float>(NFSetting::Ins().GetClientWidth());
    mScreenViewport.Height = static_cast<float>(NFSetting::Ins().GetClientHeight());
    mScreenViewport.MinDepth = 0.0f;
    mScreenViewport.MaxDepth = 1.0f;

    mScissorRect = {
        0,
        0,
        static_cast<LONG>(NFSetting::Ins().GetClientWidth()),
        static_cast<LONG>(NFSetting::Ins().GetClientHeight())
    };

    return true;
}


ID3D12Resource* const NFDXRender::CurrentBackBuffer()
{
    return mSwapChainBuffer[mCurrentBackBuffer].Get();
}


D3D12_CPU_DESCRIPTOR_HANDLE NFDXRender::DepthStencilView() const
{
    return mDsvHeap->GetCPUDescriptorHandleForHeapStart();
}


void NFDXRender::FlushCommandQueue()
{
    mCurrentFence++;

    auto _result = mCommandQueue->Signal(mFence.Get(), mCurrentFence);

    if (FAILED(_result))
    {
        MessageBox(nullptr, L"Flush command queue failed!", L"Error", MB_OK);

        return;
    }

    if (mFence->GetCompletedValue() < mCurrentFence)
    {
        HANDLE _eventHandler = CreateEventEx(
            nullptr,
            nullptr,
            false,
            EVENT_ALL_ACCESS
        );

        _result = mFence->SetEventOnCompletion(mCurrentFence, _eventHandler);

        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"SetEventOnCompletion failed!",
                L"Error",
                MB_OK
            );

            return;
        }

        WaitForSingleObject(_eventHandler, INFINITE);

        CloseHandle(_eventHandler);
    }
}


D3D12_CPU_DESCRIPTOR_HANDLE NFDXRender::CurrentBackBufferView() const
{
    auto _result = mRtvHeap->GetCPUDescriptorHandleForHeapStart();

    _result.ptr += mCurrentBackBuffer * mRtvDescriptorSize;

    return _result;
}
