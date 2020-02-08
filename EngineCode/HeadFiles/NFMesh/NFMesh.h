#pragma once

#include "NFCommonInclude.h"

using Microsoft::WRL::ComPtr;


struct NFSubmeshGeometry
{
    UINT IndexCount = 0;


    UINT StartIndexLocation = 0;


    INT BaseVertexLocation = 0;
};


class NFMesh
{
public:
    NFMesh()
    {
    }


    NFMesh(std::wstring targetName)
    {
        SetName(std::move(targetName));
    }


    std::wstring const GetName();


    void SetName(std::wstring targetName);


    D3D12_VERTEX_BUFFER_VIEW VertexBufferView() const
    {
        D3D12_VERTEX_BUFFER_VIEW _vbvDesc;

        _vbvDesc.BufferLocation = VertexBufferGPU->GetGPUVirtualAddress();
        _vbvDesc.StrideInBytes = VertexByteStride;
        _vbvDesc.SizeInBytes = VertexBufferByteSize;

        return _vbvDesc;
    }


    D3D12_INDEX_BUFFER_VIEW IndexBufferView() const
    {
        D3D12_INDEX_BUFFER_VIEW _ibvDesc;

        _ibvDesc.BufferLocation = IndexBufferGPU->GetGPUVirtualAddress();
        _ibvDesc.Format = mIndexFormat;
        _ibvDesc.SizeInBytes = IndexBufferByteSize;

        return _ibvDesc;
    }


    void DisposeUploader()
    {
        VertexBufferUploader = nullptr;

        IndexBufferUploader = nullptr;
    }


    ComPtr<ID3DBlob> VertexBufferCPU = nullptr;


    ComPtr<ID3DBlob> IndexBufferCPU = nullptr;


    ComPtr<ID3D12Resource> VertexBufferGPU = nullptr;


    ComPtr<ID3D12Resource> IndexBufferGPU = nullptr;


    ComPtr<ID3D12Resource> VertexBufferUploader = nullptr;


    ComPtr<ID3D12Resource> IndexBufferUploader = nullptr;


    std::unordered_map<std::string, NFSubmeshGeometry> DrawArgs;


    UINT VertexByteStride = 0;


    UINT VertexBufferByteSize = 0;


    UINT IndexBufferByteSize = 0;


private:

    std::wstring mName;


    DXGI_FORMAT mIndexFormat = DXGI_FORMAT_R16_UINT;
};
