#include <utility>
#include "NFMesh/NFMesh.h"


std::wstring const NFMesh::GetName()
{
    return mName;
}


void NFMesh::SetName(std::wstring targetName)
{
    mName = std::move(targetName);
}


void NFMesh::AddSubGeometry( std::wstring const & targetName, UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation)
{
    if (targetName.length() <= 0)
    {
        MessageBox(nullptr, L"Error", L"No name,please check!", MB_OK);

        return;
    }

    auto _target = mSubMeshDataMap.find(targetName);

    if (_target != mSubMeshDataMap.end())
    {
        MessageBox(nullptr, L"Error", L"Try add things, but exist!", MB_OK);

        return;
    }

    NFSubmeshGeometry _subMesh;
    ZeroMemory(&_subMesh, sizeof(_subMesh));

    _subMesh.IndexCount = indexCount;
    _subMesh.StartIndexLocation = startIndexLocation;
    _subMesh.BaseVertexLocation = baseVertexLocation;

    mSubMeshDataMap[targetName] = _subMesh;
}
