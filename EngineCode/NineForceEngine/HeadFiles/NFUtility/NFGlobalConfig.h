#pragma once
#include "NFCore/NFInclude.h"
#include "NFUtility/NFUtility.h"


namespace NineForceEngine
{
    class NFGlobalConfig final : public NFSingleton<NFGlobalConfig>
    {
    public:
        bool GetIsRunning() const;


        void SetIsRunning(bool targetValue);


        bool GetIsFullScreen() const;


        void SetIsFullScreen(bool targetValue);


        int GetScreenWidth() const;


        void SetScreenWidth(const int targetWidth);


        int GetScreenHeight() const;


        void SetScreenHeight(const int targetHeight);


        int GetRefreshRate() const;


        void SetRefreshRate(int targetValue);


        bool GetUse4XMSAA() const;


        void SetUse4XMSAA(bool targetValue);


    private:

        bool mIsFullScreen = false;


        int mScreenWidth = 1366;


        int mScreenHeight = 768;


        bool mIsRunning = false;


        int mRefreshRate = 60;


        bool mUse4XMSAA = true;
    };
}
