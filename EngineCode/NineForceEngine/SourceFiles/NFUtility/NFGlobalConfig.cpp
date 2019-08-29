#include "NFUtility/NFGlobalConfig.h"


bool NineForceEngine::NFGlobalConfig::GetIsRunning() const
{
    return mIsRunning;
}


void NineForceEngine::NFGlobalConfig::SetIsRunning(bool targetValue)
{
    mIsRunning = targetValue;
}


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


int NineForceEngine::NFGlobalConfig::GetRefreshRate() const
{
    return mRefreshRate;
}


void NineForceEngine::NFGlobalConfig::SetRefreshRate(int targetValue)
{
    mRefreshRate = targetValue;
}


bool NineForceEngine::NFGlobalConfig::GetUse4XMSAA() const
{
    return mUse4XMSAA;
}


void NineForceEngine::NFGlobalConfig::SetUse4XMSAA(bool targetValue)
{
    mUse4XMSAA = targetValue;
}
