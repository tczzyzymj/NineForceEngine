#pragma once
#include "NFInclude.h"
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


        void Resize();


        bool Update(float deltaTime) const;


        void Clean();


    private:


        bool mIsShutDown = false;


        NFWindow* mWindow = nullptr;


        NFRender* mRender = nullptr;


        bool mHasInit = false;
    };
}
