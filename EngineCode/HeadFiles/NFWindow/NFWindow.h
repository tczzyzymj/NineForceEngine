#pragma once
#include "NFCommonInclude.h"
#include "NFDXRender/NFDXRender.h"


class NFWindow
{
public:
    NFWindow(HINSTANCE hIns);


    // init for window
    bool Init();


    bool InitWindow();


    bool InitD3D();


    void Update(float deltaTime);


    void Render(float deltaTime);


    int Run();


    void OnResize();


    static NFWindow* GetWindowPtr();


    virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

    NFDXRender* mDXRender;


    static NFWindow* mNFWindow;


    HINSTANCE mIns;


    HWND mWnd;


    std::wstring mMainWndCaption = L"NF Window";
};
