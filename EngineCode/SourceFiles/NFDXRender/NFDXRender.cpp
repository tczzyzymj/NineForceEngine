#include "NFDXRender/NFDXRender.h"


using Microsoft::WRL::ComPtr;


NFDXRender::NFDXRender()
{
}


bool NFDXRender::Init()
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
        mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_RTV
        );

        mDSVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
            D3D12_DESCRIPTOR_HEAP_TYPE_DSV
        );

        mCBVSRVUAVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(
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

        m4XMSAAQuality = _msQualityLevels.NumQualityLevels;

        assert(m4XMSAAQuality > 0 && "Unexpected MSAA quality level.");
    }

    return true;
}
