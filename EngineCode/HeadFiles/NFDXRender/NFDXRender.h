#pragma once
#include "NFCommonInclude.h"


using Microsoft::WRL::ComPtr;


class NFDXRender
{
public:
    NFDXRender();


    bool Init(HWND targetHwnd);


    void LogAdapters();


    bool CreateCommandObjects();


    bool CreateSwapChain(HWND targetHwnd);


    bool CreateRtvAndDsvDescriptionHeaps();


    void Render();


private:
    ComPtr<IDXGIFactory4> mFactory = nullptr;


    ComPtr<IDXGISwapChain> mSwapChain = nullptr;


    ComPtr<ID3D12Device> mDevice = nullptr;


    ComPtr<ID3D12Fence> mFence = nullptr;


    ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;


    ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc = nullptr;


    ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;


    ComPtr<ID3D12DescriptorHeap> mRtvHeap;


    ComPtr<ID3D12DescriptorHeap> mDsvHeap;


    UINT mRtvDescriptorSize = 0;


    UINT mDsvDescriptorSize = 0;


    UINT mCbvSrvUavDescriptorSize = 0;


    UINT m4sMsaaQuality = 0;


    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;


    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


    static const UINT mSwapChainBufferCount = 2;
};
