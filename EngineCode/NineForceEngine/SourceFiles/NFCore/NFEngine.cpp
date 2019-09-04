#include "NFCore/NFEngine.h"
#include "NFUtility/NFGlobalConfig.h"
#include "NFCore/NFTimer.h"


NineForceEngine::NFEngine::NFEngine()
{
    mRender = new NFRender();

    mWindow = new NFWindow();
}


NineForceEngine::NFEngine::~NFEngine()
{
}


bool NineForceEngine::NFEngine::Init()
{
    if (mHasInit)
    {
        return true;
    }

    if (!mWindow->Init())
    {
        return false;
    }

    if (!mRender->Init(mWindow->GetWindowHandler()))
    {
        return false;
    }

    if (!NFTimer::Instance()->Init())
    {
        return false;
    }

    mHasInit = true;

    NFGlobalConfig::Instance()->SetIsRunning(true);

    return true;
}


void NineForceEngine::NFEngine::Resize()
{
}


bool NineForceEngine::NFEngine::Update(float deltaTime) const
{
    if (!mHasInit)
    {
        return false;
    }

    if (mIsShutDown)
    {
        return false;
    }

    mWindow->Update(deltaTime);

    mRender->Update(deltaTime);

    return true;
}


void NineForceEngine::NFEngine::Clean()
{
    if (mRender != nullptr)
    {
        mRender->Clean();

        delete mRender;

        mRender = nullptr;
    }

    if (mWindow != nullptr)
    {
        mWindow->Clean();

        delete mWindow;

        mWindow = nullptr;
    }
}
