#pragma once
#include "NFInclude.h"
#include "NFInput.h"
#include "NFRender.h"


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


        LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


    private:


        bool mIsShutDown = false;


        NFInput* mInput = nullptr;


        NFRender* mRender = nullptr;


        bool mHasInit = false;
    };
}
