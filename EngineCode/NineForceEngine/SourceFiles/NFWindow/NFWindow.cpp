#include "NFWindow/NFWindow.h"
#include "NFUtility/NFGlobalConfig.h"


NineForceEngine::NFWindow::NFWindow()
{
}


NineForceEngine::NFWindow::~NFWindow()
{
}


bool NineForceEngine::NFWindow::Init(int screenWidth, int screenHeight)
{
    WNDCLASSEX _wc;
    DEVMODE _dmScreenSettings;
    int _posX, _posY;


    // Get an external pointer to this object.
    // ApplicationHandle = this;

    // Get the instance of this application.
    mHandleInstance = GetModuleHandle(nullptr);

    // Give the application a name.
    m_applicationName = L"Engine";

    // Setup the windows class with default settings.
    _wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    _wc.lpfnWndProc = WndProc;
    _wc.cbClsExtra = 0;
    _wc.cbWndExtra = 0;
    _wc.hInstance = mHandleInstance;
    _wc.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    _wc.hIconSm = _wc.hIcon;
    _wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    _wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    _wc.lpszMenuName = nullptr;
    _wc.lpszClassName = m_applicationName;
    _wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    RegisterClassEx(&_wc);

    // Determine the resolution of the clients desktop screen.
    screenWidth = GetSystemMetrics(SM_CXSCREEN);
    screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
    if (NFGlobalConfig::Instance()->GetIsFullScreen())
    {
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));
        _dmScreenSettings.dmSize = sizeof(_dmScreenSettings);
        _dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(screenWidth);
        _dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(screenHeight);
        _dmScreenSettings.dmBitsPerPel = 32;
        _dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        // Change the display settings to full screen.
        ChangeDisplaySettings(&_dmScreenSettings, CDS_FULLSCREEN);

        // Set the position of the window to the top left corner.
        _posX = _posY = 0;
    }
    else
    {
        // If windowed then set it to 800x600 resolution.
        screenWidth = 800;
        screenHeight = 600;

        // Place the window in the middle of the screen.
        _posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
        _posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
    }

    // Create the window with the screen settings and get the handle to it.
    mHwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        m_applicationName,
        m_applicationName,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
        _posX,
        _posY,
        screenWidth,
        screenHeight,
        nullptr,
        nullptr,
        mHandleInstance,
        nullptr
    );

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(mHwnd, SW_SHOW);

    SetForegroundWindow(mHwnd);

    SetFocus(mHwnd);

    // Hide the mouse cursor.
    ShowCursor(false);

    return true;
}


LRESULT NineForceEngine::NFWindow::MessageHandler(HWND, UINT, WPARAM, LPARAM)
{
    return 0;
}


LRESULT NineForceEngine::NFWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
            // Check if the window is being destroyed.
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }

            // Check if the window is being closed.
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }

            // All other messages pass to the message handler in the system class.
        default:
        {
            return 0;
            //return ApplicationHandle->MessageHandler(hwnd, message, wparam, lparam);
        }
    }
}
