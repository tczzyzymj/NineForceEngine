#include "NFUtility/NFGlobalConfig.h"


bool NineForceEngine::NFGlobalConfig::GetIsFullScreen() const
{
    return mIsFullScreen;
}


void NineForceEngine::NFGlobalConfig::SetIsFullScreen(const bool targetValue)
{
    mIsFullScreen = targetValue;
}


int NineForceEngine::NFGlobalConfig::GetScreenWidth() const
{
    return mScreenWidth;
}


void NineForceEngine::NFGlobalConfig::SetScreenWidth(const int targetWidth)
{
    mScreenWidth = targetWidth;
}


int NineForceEngine::NFGlobalConfig::GetScreenHeight() const
{
    return mScreenHeight;
}


void NineForceEngine::NFGlobalConfig::SetScreenHeight(const int targetHeight)
{
    mScreenHeight = targetHeight;
}
