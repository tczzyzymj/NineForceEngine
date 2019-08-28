#pragma once
#include "NFCore/NFInclude.h"
#include "NFUtility/NFUtility.h"


namespace NineForceEngine
{
    class NFGlobalConfig : public NFSingleton<NFGlobalConfig>
    {
    private:


        int mScreenHeight = 1366;


        int mScreenWidth = 768;


    public:
        int GetHeight() const
        {
            return mScreenHeight;
        }


        int GetWidth() const
        {
            return mScreenWidth;
        }
    };
}
