#pragma once
#include "NFCommonInclude.h"

using namespace Microsoft::WRL;


template <typename T> class NFUploadBuffer
{
public :
    NFUploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer)
    {
        mIsConstantBuffer = isConstantBuffer;

        if (mIsConstantBuffer)
        {
            mElementSize = NFUtility::Ins().CalcConstantBufferByteSize(sizeof(T));
        }
        else
        {
            mElementSize = sizeof(T);
        }

        D3D12_HEAP_PROPERTIES _heapProperties = {};

        ZeroMemory(&_heapProperties, sizeof(D3D12_HEAP_PROPERTIES));

        _heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
        _heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
        _heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
        _heapProperties.CreationNodeMask = 1;
        _heapProperties.VisibleNodeMask = 1;

        D3D12_RESOURCE_DESC _resourceDesc = {};

        ZeroMemory(&_resourceDesc, sizeof(_resourceDesc));

        _resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
        _resourceDesc.Alignment = 0;
        _resourceDesc.Width = mElementSize * elementCount;
        _resourceDesc.Height = 1;
        _resourceDesc.DepthOrArraySize = 1;
        _resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
        _resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
        _resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
        _resourceDesc.SampleDesc.Count = 1;
        _resourceDesc.SampleDesc.Quality = 0;
        _resourceDesc.MipLevels = 1;

        ThrowIfFailed(
            device->CreateCommittedResource(
                &_heapProperties,
                D3D12_HEAP_FLAG_NONE,
                &_resourceDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&mUploadBuffer)
            )
        );

        ThrowIfFailed(
            mUploadBuffer->Map(
                0,
                nullptr,
                reinterpret_cast<void**>(mMappedData)
            )
        );
    }


    NFUploadBuffer(const NFUploadBuffer& other) = delete;


    NFUploadBuffer operator=(const NFUploadBuffer& other) = delete;


    ~NFUploadBuffer()
    {
        if (mUploadBuffer != nullptr)
        {
            mUploadBuffer->Unmap(0, nullptr);
        }

        mMappedData = nullptr;
    }


    ID3D12Resource* Resource() const
    {
        return mUploadBuffer.Get();
    }


private:
    ComPtr<ID3D12Resource> mUploadBuffer = nullptr;


    bool mIsConstantBuffer;


    BYTE* mMappedData = nullptr;


    UINT mElementSize = 0;
};
