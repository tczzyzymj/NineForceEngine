#include "NFUtility.h"


NFException::NFException(
    HRESULT targetResult,
    const std::wstring& functionName,
    const std::wstring& fileName,
    UINT lineNumber)
{
    mFileName = fileName;

    mLineNumber = lineNumber;

    mResult = targetResult;

    mFunctionName = functionName;
}


std::wstring NFException::ToString() const
{
    _com_error _error(mResult);

    std::wstring _msg = _error.ErrorMessage();

    return L"Call [" + mFunctionName + L"] failed ! \n" +
        L" In : [" + mFileName + L"] ; Line : " + std::to_wstring(mLineNumber) +
        L".\n Error message : " + _msg;
}


wchar_t* NFUtility::AnsiToUnicode(const char* szStr)
{
    auto _length = MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        szStr,
        -1,
        nullptr,
        0
    );

    if (_length == 0)
    {
        return nullptr;
    }

    auto _resultPtr = new wchar_t[_length];

    MultiByteToWideChar(
        CP_ACP,
        MB_PRECOMPOSED,
        szStr,
        -1,
        _resultPtr,
        _length
    );

    return _resultPtr;
}


char* NFUtility::UnicodeToAnsi(const wchar_t* szStr)
{
    auto _length = WideCharToMultiByte(
        CP_ACP,
        0,
        szStr,
        -1,
        nullptr,
        0,
        nullptr,
        nullptr
    );

    if (_length == 0)
    {
        return nullptr;
    }

    auto _resultPtr = new char[_length];

    WideCharToMultiByte(
        CP_ACP,
        0,
        szStr,
        -1,
        _resultPtr,
        _length,
        nullptr,
        nullptr
    );

    return _resultPtr;
}

