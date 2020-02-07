#pragma once

#include "NFCommonInclude.h"
#include "NFSetting.h"

using namespace Microsoft::WRL;


class NFShaderManager
{
public:
    NFShaderManager();

    ComPtr<ID3DBlob> CompileShader(
        const std::wstring& filePath,
        const std::string& entryPoint,
        const std::string& target,
        const D3D_SHADER_MACRO* defines
    );


    bool Init();
};
