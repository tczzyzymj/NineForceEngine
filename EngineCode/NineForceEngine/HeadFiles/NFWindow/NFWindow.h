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


        void OnResize();


        void Update(float deltaTime);


        void Clean();


        HWND GetWindowHandler() const;


        static LRESULT CALLBACK MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


    private:
        HINSTANCE mHandleInstance{};


        HWND mHwnd{};


        bool mHasInit = false;


        LPCWSTR mApplicationName = nullptr;


        static NFWindow* mIns;


        bool InitWindow();
    };
}
