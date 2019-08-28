#include "NFCore/NFEngine.h"


NineForceEngine::NFEngine::NFEngine()
{
    mInput = new NFInput();

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

    if (!mInput->Init())
    {
        return false;
    }

    if (!mRender->Init())
    {
        return false;
    }

    mHasInit = true;

    return true;
}


bool NineForceEngine::NFEngine::Update() const
{
    if (!mHasInit)
    {
        return false;
    }

    if (mIsShutDown)
    {
        return false;
    }

    const auto _deltaTime = 0.033f;

    mInput->Update(_deltaTime);

    mRender->Update(_deltaTime);

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

    if (mInput != nullptr)
    {
        mInput->Clean();

        delete mInput;

        mInput = nullptr;
    }
}
