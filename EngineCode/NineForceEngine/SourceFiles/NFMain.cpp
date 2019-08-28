#include "Core/NFEngine.h"

using namespace NineForceEngine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    auto _system = new NFEngine();

    if (!_system->Init())
    {
        return -1;
    }

    while (true)
    {
        if (!_system->Update())
        {
            break;
        }
    }

    _system->Clean();

    delete _system;

    _system = nullptr;

    return 0;
}
