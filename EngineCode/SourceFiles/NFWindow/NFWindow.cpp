#include "NFWindow/NFWindow.h"


NFWindow::NFWindow()
{
}


int NFWindow::Update()
{
    MSG _msg = {nullptr};

    while (_msg.message != WM_QUIT)
    {
        if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&_msg);

            DispatchMessage(&_msg);
        }
        else
        {
            Update();

            Draw();
        }
    }

    return static_cast<int>(_msg.wParam);
}


void NFWindow::Draw()
{
}


bool NFWindow::Init(HINSTANCE hIns)
{
    return true;
}
