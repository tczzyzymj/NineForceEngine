#pragma once
#include "NFCommonInclude.h"
#include <vector>
#include "NFShader/NFShaderManager.h"
#include "NFBuffer/NFUploadBuffer.h"
#include "NFMesh/NFMesh.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;


struct NFObjectConstants
{
    DirectX::XMFLOAT4X4 WorldViewProj;
};


struct NFVertex
{
    DirectX::XMFLOAT3 Pos;


    DirectX::XMFLOAT4 Color;
};


class NFDXRender
{
public:
    NFDXRender();


    bool Init(HWND targetHwnd);


    void LogAdapters();


    bool CreateCommandObjects();


    bool CreateSwapChain(HWND targetHwnd);


    bool BuildRtvAndDsvDescriptionHeaps();


    bool BuildCbvDescriptionHeaps();


    bool BuildConstantBuffer();


    bool BuildRootSignature();


    bool BuildShadersAndInputLayout();


    bool BuildBoxGeometry();


    bool InitShader();


    bool BuildPipelineState();


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


    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;


    D3D12_VIEWPORT mScreenViewport;


    D3D12_RECT mScissorRect;


    UINT64 mCurrentFence = 0;


    NFShaderManager* mShaderManager = nullptr;


    ComPtr<IDXGIFactory4> mFactory = nullptr;


    ComPtr<IDXGISwapChain> mSwapChain = nullptr;


    ComPtr<ID3D12Device> mDevice = nullptr;


    ComPtr<ID3D12Fence> mFence = nullptr;


    ComPtr<ID3D12CommandQueue> mCommandQueue = nullptr;


    ComPtr<ID3D12CommandAllocator> mCommandAllocator = nullptr;


    ComPtr<ID3D12GraphicsCommandList> mCommandList = nullptr;


    ComPtr<ID3D12PipelineState> mPipeLineState = nullptr;


    ComPtr<ID3D12DescriptorHeap> mRtvHeap = nullptr;


    ComPtr<ID3D12DescriptorHeap> mDsvHeap = nullptr;


    ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;


    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;


    ComPtr<ID3DBlob> mVertexShader = nullptr;


    ComPtr<ID3DBlob> mPixShader = nullptr;


    ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];


    ComPtr<ID3D12Resource> mDepthStencilBuffer = nullptr;


    std::unique_ptr<NFUploadBuffer<NFObjectConstants>> mObjCB;


    std::unique_ptr<NFMesh> mBoxMesh;
};
