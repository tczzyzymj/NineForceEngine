#pragma once
#include "NFInclude.h"
#include "NFInput.h"
#include "NFRender.h"

namespace NineForceEngine
{
    class NFEngine final
    {
    public:

        explicit NFEngine();


        ~NFEngine();


        bool Init() const;


        bool Update() const;


    private:

        NFInput* mInput = nullptr;


        NFRender* mRender = nullptr;


        bool mHasInit = false;
    };
}