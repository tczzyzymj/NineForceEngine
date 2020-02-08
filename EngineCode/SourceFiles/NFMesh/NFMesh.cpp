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
