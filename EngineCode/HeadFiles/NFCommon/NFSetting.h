#pragma once
#include "NFCommonInclude.h"


class NFSetting
{
public:

    static NFSetting& Ins()
    {
        static NFSetting mIns;

        return mIns;
    }


    NFSetting(NFSetting const&) = delete;


    NFSetting operator=(NFSetting const&) = delete;


    UINT GetClientHeight();


    UINT GetClientWidth();


    void SetClientWidth(UINT targetValue);


    void SetClientHeight(UINT targetValue);


    void SetEnable4xMsaa(bool isEnable);


    bool GetEnable4xMsaa();


private :
    NFSetting()
    {
    }


    UINT mClientHeight = 768;


    UINT mClientWidth = 1366;


    bool mEnable4XMSAA = false;
};


inline UINT NFSetting::GetClientHeight()
{
    return mClientHeight;
}


inline UINT NFSetting::GetClientWidth()
{
    return mClientWidth;
}


inline void NFSetting::SetClientWidth(UINT targetValue)
{
    mClientWidth = targetValue;
}


inline void NFSetting::SetClientHeight(UINT targetValue)
{
    mClientHeight = targetValue;
}


inline void NFSetting::SetEnable4xMsaa(bool isEnable)
{
    mEnable4XMSAA = isEnable;
}


inline bool NFSetting::GetEnable4xMsaa()
{
    return mEnable4XMSAA;
}
