#pragma once
#include "NFInclude.h"
#include <d3d11.h>


namespace NineForceEngine
{
    class NFRender final
    {
    public:
        explicit NFRender();


        ~NFRender();


        bool Init();


        void Update(float deltaTime);


        void Clean();


    private:

        void Render();


        ID3D11Device* mDevice = nullptr;


        ID3D11DeviceContext* mContext = nullptr;
    };
}
