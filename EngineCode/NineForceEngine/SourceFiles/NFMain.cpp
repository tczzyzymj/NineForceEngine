#include "NFCore/NFEngine.h"
#include "NFUtility/NFGlobalConfig.h"
#include "NFCore/NFTimer.h"
#include <string>


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

    while (_globalIns->GetIsRunning())
    {
        _timerIns->BeginRecord();


        int a = 0;

        while (a < 1000000)
        {
            ++a;
        }

        const auto _result = _engine->Update(_timerIns->GetDeltaTime());

        _timerIns->EndRecord();

        const auto _deltaTime = _timerIns->GetDeltaTime();

        std::string _errorMsg = "DeltaTime is : " + std::to_string(_deltaTime);

        MessageBox(nullptr, LPCWSTR(_errorMsg.c_str()), L"Error", 0);

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
