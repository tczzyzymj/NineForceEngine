#pragma once
#include "NFCore/NFInclude.h"


namespace NineForceEngine
{
    /**
    * \brief 
    * this is for window, there may be different platform, use different window
    */
    class NFWindow final
    {
    public:
        NFWindow();


        ~NFWindow();


        bool Init();


        LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


    private:
    };
}
