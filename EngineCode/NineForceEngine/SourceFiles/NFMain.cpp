#include "NFCore/NFEngine.h"
#include "NFUtility/NFGlobalConfig.h"
#include "NFCore/NFTimer.h"
#include <string>
#include "NFUtility/NFUtility.h"
#include <stdio.h>


using namespace NineForceEngine;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    auto _engine = new NFEngine();

    if (!_engine->Init())
    {
        return -1;
    }

    const auto _globalIns = NFGlobalConfig::Instance();

    auto _timerIns = NFTimer::Instance();

    _timerIns->Init();

    while (_globalIns->GetIsRunning())
    {
        _timerIns->BeginRecord();

        const auto _result = _engine->Update(static_cast<float>(_timerIns->GetDeltaTime()));

        _timerIns->EndRecord();

        if (!_result)
        {
            break;
        }
    }

    _engine->Clean();

    delete _engine;

    _engine = nullptr;

    return 0;
}
