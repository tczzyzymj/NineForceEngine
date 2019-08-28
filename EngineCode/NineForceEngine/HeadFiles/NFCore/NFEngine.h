#pragma once
#include "NFInclude.h"
#include "NFInput.h"
#include "NFRender.h"
#include "NFWindow/NFWindow.h"


namespace NineForceEngine
{
    class NFEngine
    {
    public:

        explicit NFEngine();


        ~NFEngine();


        bool Init();


        bool Update() const;


        void Clean();


    private:


        bool mIsShutDown = false;


        NFInput* mInput = nullptr;


        NFRender* mRender = nullptr;


        NFWindow* mWindow = nullptr;


        bool mHasInit = false;
    };
}
