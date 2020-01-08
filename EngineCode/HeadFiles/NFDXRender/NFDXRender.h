#pragma once
#include "NFCommonInclude.h"


using Microsoft::WRL::ComPtr;



class NFDXRender
{
public:
    NFDXRender();


    bool Init();


private:
    ComPtr<IDXGIFactory4> mFactory = nullptr;

    ComPtr<IDXGISwapChain> mSwapChain = nullptr;

    ComPtr<ID3D12Device> mDevice = nullptr;

    ComPtr<ID3D12Fence> mFence = nullptr;

    ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;

    UINT mRTVDescriptorSize = 0;

    UINT mDSVDescriptorSize = 0;

    UINT mCBVSRVUAVDescriptorSize = 0;

    UINT m4XMSAAQuality = 0;

    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;

    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
};
