#pragma once

#include "NFCommonInclude.h"
#include "comdef.h"
#include <windows.h>


class NFUtility
{
public:
    static NFUtility& GetInstance()
    {
        static NFUtility mIns;

        return mIns;
    }


    NFUtility(NFUtility const&) = delete;


    NFUtility& operator=(NFUtility const&) = delete;


    void ThrowIfFailed(HRESULT targetResult)
    {
        if (FAILED(targetResult))
        {
            _com_error _error(targetResult);

            std::wstring _msg = _error.ErrorMessage();

            auto _finalMsg = UnicodeToAnsi(_msg.c_str());

            throw std::exception(_finalMsg);
        }
    }


    wchar_t* AnsiToUnicode(const char* szStr)
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


    //将宽字节wchar_t*转化为单字节char*  
    char* UnicodeToAnsi(const wchar_t* szStr)
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


private:
    NFUtility()
    {
    }
};
