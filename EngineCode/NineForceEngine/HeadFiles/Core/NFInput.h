#pragma once
#include "NFInclude.h"


namespace NineForceEngine
{
    class NFInput
    {
    public:
        explicit NFInput();


        ~NFInput();


        bool Init();


        void Update(float deltaTime);


        void Clean();


    private:
        bool mHasInit = false;
    };
}
