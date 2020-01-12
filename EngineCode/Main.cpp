#include "NFCommonInclude.h"
#include "NFWindow/NFWindow.h"


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
    catch (std::exception _e)
    {
        MessageBox(
            nullptr,
            NFUtility::GetInstance().AnsiToUnicode(_e.what()),
            L"Error",
            MB_OK
        );

        return 0;
    }
}
