#include "NFCommonInclude.h";
#include "NFWindow/NFWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    auto* _window = new NFWindow();

    if (!_window->Init(hInstance))
    {
        return -1;
    }

    return _window->Update();
}
