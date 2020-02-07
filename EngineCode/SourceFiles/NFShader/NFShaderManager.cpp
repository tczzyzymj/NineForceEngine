#include "NFShader/NFShaderManager.h"


NFShaderManager::NFShaderManager()
{
}


ComPtr<ID3DBlob> NFShaderManager::CompileShader(
    const std::wstring& filePath,
    const std::string& entryPoint,
    const std::string& target,
    const D3D_SHADER_MACRO* defines
)
{
    UINT _compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    _compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    ComPtr<ID3DBlob> _byteCode = nullptr;
    ComPtr<ID3DBlob> _errorBlob = nullptr;

    auto _tempResult = D3DCompileFromFile(
        filePath.c_str(),
        defines,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        entryPoint.c_str(),
        target.c_str(),
        _compileFlags,
        0,
        &_byteCode,
        &_errorBlob
    );

    if (_errorBlob != nullptr)
    {
#ifdef UNICODE
        OutputDebugStringW(static_cast<wchar_t*>(_errorBlob->GetBufferPointer()));
#else
        OutputDebugStringA(static_cast<char*>(_errorBlob->GetBufferPointer()));
#endif
    }

    ThrowIfFailed(_tempResult);

    return _byteCode;
}


bool NFShaderManager::Init()
{
    return true;
}
