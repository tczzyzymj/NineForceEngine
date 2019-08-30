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


int NineForceEngine::NFGlobalConfig::GetResolutionWidth() const
{
    return mResolutionWidth;
}


void NineForceEngine::NFGlobalConfig::SetResolutionWidth(const int targetWidth)
{
    mResolutionWidth = targetWidth;
}


int NineForceEngine::NFGlobalConfig::GetResolutionHeight() const
{
    return mResolutionHeight;
}


void NineForceEngine::NFGlobalConfig::SetResolutionHeight(const int targetHeight)
{
    mResolutionHeight = targetHeight;
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
