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


    bool BuildPipleState();


    bool Render();


    bool OnResize();

    ID3D12Resource* const CurrentBackBuffer();


    D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;


    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;


    void FlushCommandQueue();


private:

    UINT mRtvDescriptorSize = 0;


    UINT mDsvDescriptorSize = 0;


    UINT mCbvSrvUavDescriptorSize = 0;


    UINT m4xMsaaQuality = 0;


    UINT mCurrentBackBuffer = 0;


    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;


    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;


    static const UINT mSwapChainBufferCount = 2;


    D3D12_VIEWPORT mScreenViewport;


    D3D12_RECT mScissorRect;


    UINT64 mCurrentFence = 0;


    ComPtr<IDXGIFactory4> mFactory = nullptr;


    ComPtr<IDXGISwapChain> mSwapChain = nullptr;


    ComPtr<ID3D12Device> mDevice = nullptr;


    ComPtr<ID3D12Fence> mFence = nullptr;


    ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;


    ComPtr<ID3D12CommandAllocator> mCommandAllocator = nullptr;


    ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;


    ComPtr<ID3D12PipelineState> mPipeLineState = nullptr;


    ComPtr<ID3D12DescriptorHeap> mRtvHeap;


    ComPtr<ID3D12DescriptorHeap> mDsvHeap;


    ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];


    ComPtr<ID3D12Resource> mDepthStencilBuffer;
};
