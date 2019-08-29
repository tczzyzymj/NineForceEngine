#include "NFWindow/NFWindow.h"
#include "NFUtility/NFGlobalConfig.h"


NineForceEngine::NFWindow::NFWindow()
{
}


NineForceEngine::NFWindow::~NFWindow()
{
}


void NineForceEngine::NFWindow::Update(float deltaTime)
{
    MSG _msg;

    ZeroMemory(&_msg, sizeof(_msg));

    auto _done = false;

    while (!_done)
    {
        if (PeekMessage(
            &_msg,
            nullptr,
            0,
            0,
            PM_REMOVE))
        {
            TranslateMessage(&_msg);

            DispatchMessage(&_msg);
        }

        if (_msg.message == WM_QUIT)
        {
            _done = true;

            NFGlobalConfig::Instance()->SetIsRunning(false);
        }
    }
}


void NineForceEngine::NFWindow::Clean()
{
    // Show the mouse cursor.
    ShowCursor(true);

    // Fix the display settings if leaving full screen mode.
    if (NFGlobalConfig::Instance()->GetIsFullScreen())
    {
        ChangeDisplaySettings(nullptr, 0);
    }

    // Remove the window.
    DestroyWindow(mHwnd);

    mHwnd = nullptr;

    // Remove the application instance.
    UnregisterClass(mApplicationName, mHandleInstance);

    mHandleInstance = nullptr;

    // Release the pointer to this class.
    mApplicationName = nullptr;
}


bool NineForceEngine::NFWindow::Init(int screenWidth, int screenHeight)
{
    if (!InitWindow(screenWidth, screenHeight))
    {
        return false;
    }

    mHasInit = true;

    return true;
}


LRESULT NineForceEngine::NFWindow::MessageHandler(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
            // Check if a key has been pressed on the keyboard.
        case WM_KEYDOWN:
        {
            // If a key is pressed send it to the input object so it can record that state.
            // mInput->KeyDown(static_cast<unsigned int>(wParam));

            return 0;
        }

            // Check if a key has been released on the keyboard.
        case WM_KEYUP:
        {
            // If a key is released then send it to the input object so it can unset the state for that key.
            //mInput->KeyUp(static_cast<unsigned int>(wParam));

            return 0;
        }


        default:
        {
            // Any other messages send to the default message handler as our application won't make use of them.

            return DefWindowProc(hwnd, message, wParam, lParam);
        }
    }
}


LRESULT NineForceEngine::NFWindow::WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message)
    {
        case WM_DESTROY:
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }
        default:
        {
            return MessageHandler(hwnd, message, wparam, lparam);
        }
    }
}


bool NineForceEngine::NFWindow::InitWindow(int screenWidth, int screenHeight)
{
    WNDCLASSEX _wc;
    DEVMODE _dmScreenSettings;
    int _posX, _posY;


    // Get an external pointer to this object.
    // ApplicationHandle = this;

    // Get the instance of this application.
    mHandleInstance = GetModuleHandle(nullptr);

    // Give the application a name.
    mApplicationName = L"NineForceEngine";

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
    _wc.lpszClassName = mApplicationName;
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
        mApplicationName,
        mApplicationName,
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
