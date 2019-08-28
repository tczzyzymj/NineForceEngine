#pragma once
#include "NFInclude.h"


namespace NineForceEngine
{
    class NFRender final
    {
    public:
        explicit NFRender();


        ~NFRender();


        bool Init() const;


        void Update(float deltaTime);


        void Clean();


    private:

        void Render();
    };
}
