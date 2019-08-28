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


        bool Init(int screenWidth, int screenHeight);


        LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);


        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);


    private:
        HINSTANCE mHandleInstance;


        HWND mHwnd;


        LPCWSTR m_applicationName = nullptr;
    };
}
