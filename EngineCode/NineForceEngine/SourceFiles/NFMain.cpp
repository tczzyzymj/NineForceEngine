#include "NFCore/NFEngine.h"
#include "NFUtility/NFGlobalConfig.h"


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

    auto _ins = NFSingleton<NFGlobalConfig>::Ins();
    
    _system->Clean();

    delete _system;

    _system = nullptr;

    return 0;
}
