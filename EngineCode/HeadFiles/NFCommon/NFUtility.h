#pragma once

#include "NFCommonInclude.h"
#include "comdef.h"
#include <windows.h>


class NFException : std::exception
{
public:
    NFException() = default;


    NFException(
        HRESULT targetResult,
        const std::wstring& functionName,
        const std::wstring& fileName,
        UINT lineNumber
    );


    std::wstring ToString() const;


private:
    HRESULT mResult;


    std::wstring mFileName;


    std::wstring mFunctionName;


    UINT mLineNumber = 0;
};

#ifndef ThrowIfFailed
#define ThrowIfFailed(x)                                                        \
{                                                                               \
    HRESULT _result = (x);                                                         \
    std::wstring wfn = NFUtility::Ins().AnsiToUnicode(__FILE__);        \
    if(FAILED(_result)) { throw NFException(_result, L#x, wfn, __LINE__); }           \
}
#endif

class NFUtility
{
public:
    static NFUtility& Ins()
    {
        static NFUtility mIns;

        return mIns;
    }


    NFUtility(NFUtility const&) = delete;


    NFUtility& operator=(NFUtility const&) = delete;


    wchar_t* AnsiToUnicode(const char* szStr);


    //将宽字节wchar_t*转化为单字节char*  
    char* UnicodeToAnsi(const wchar_t* szStr);


    UINT CalcConstantBufferByteSize(UINT targetSize);


private:
    NFUtility()
    {
    }
};
