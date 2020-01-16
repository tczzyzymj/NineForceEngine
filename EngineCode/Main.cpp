#include "NFCommonInclude.h"
#include "NFWindow/NFWindow.h"
#include "NFUtility.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
    try
    {
        auto* _window = new NFWindow(hInstance);

        if (!_window->Init())
        {
            return 0;
        }

        return _window->Run();
    }
    catch (NFException& _exception)
    {
        MessageBox(
            nullptr,
            _exception.ToString().c_str(),
            L"Error",
            MB_OK
        );

        return 0;
    }
}
