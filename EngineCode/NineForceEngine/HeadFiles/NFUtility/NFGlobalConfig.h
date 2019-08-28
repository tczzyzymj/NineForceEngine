#pragma once
#include "NFCore/NFInclude.h"
#include "NFUtility/NFUtility.h"


namespace NineForceEngine
{
    class NFGlobalConfig final : public NFSingleton<NFGlobalConfig>
    {
    private:
        friend NFSingleton<NFGlobalConfig>;


        bool mIsFullScreen = false;


        int mScreenWidth = 1366;


        int mScreenHeight = 768;


    public:
        bool GetIsFullScreen() const;


        void SetIsFullScreen(bool targetValue);


        int GetScreenWidth() const;


        void SetScreenWidth(const int targetWidth);


        int GetScreenHeight() const;


        void SetScreenHeight(const int targetHeight);
    };
}
