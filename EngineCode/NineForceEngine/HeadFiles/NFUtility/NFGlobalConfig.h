#pragma once
#include "NFCore/NFInclude.h"
#include "NFCore/NFSingleton.h"


namespace NineForceEngine
{
    class NFGlobalConfig final : public NFSingleton<NFGlobalConfig>
    {
    public:
        bool GetIsRunning() const;


        void SetIsRunning(bool targetValue);


        bool GetIsFullScreen() const;


        void SetIsFullScreen(bool targetValue);


        int GetResolutionWidth() const;


        void SetResolutionWidth(const int targetWidth);


        int GetResolutionHeight() const;


        void SetResolutionHeight(const int targetHeight);


        int GetRefreshRate() const;


        void SetRefreshRate(int targetValue);


        bool GetUse4XMSAA() const;


        void SetUse4XMSAA(bool targetValue);


    private:

        bool mIsFullScreen = false;


        int mResolutionWidth = 1366;


        int mResolutionHeight = 768;


        bool mIsRunning = false;


        int mRefreshRate = 60;


        bool mUse4XMSAA = true;
    };
}
