#include "Core\NFEngine.h"


NineForceEngine::NFEngine::NFEngine()
{
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

    mInput = new NFInput();

    mRender = new NFRender();

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


LRESULT NineForceEngine::NFEngine::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
    return LRESULT();
}
