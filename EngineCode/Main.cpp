#include "NFCommonInclude.h";
#include "NFWindow/NFWindow.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    auto* _window = new NFWindow(hInstance);

    if (!_window->Init())
    {
        return -1;
    }

    return _window->Run();
}
