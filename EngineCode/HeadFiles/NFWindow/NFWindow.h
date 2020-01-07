#pragma once
#include "NFCommonInclude.h"


class NFWindow
{
public:
    NFWindow(HINSTANCE hIns);


    // init for window
    bool Init();


    bool InitWindow();


    bool InitD3D() const;


    void Update();

    int Run();


    void Draw();


    static NFWindow* GetWindowPtr();


    virtual LRESULT MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);


private:

    static NFWindow* mNFWindow;


    HINSTANCE mIns;


    int mClientWidth = 1366;


    int mClientHeight = 768;


    HWND mWnd;


    std::wstring mMainWndCaption = L"NF Window";
};
