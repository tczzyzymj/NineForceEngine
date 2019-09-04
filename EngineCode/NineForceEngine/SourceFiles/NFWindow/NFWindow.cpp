#include "NFWindow/NFWindow.h"
#include "NFUtility/NFGlobalConfig.h"
#include "NFCore/NFEngine.h"


NineForceEngine::NFWindow* NineForceEngine::NFWindow::mIns;


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


HWND NineForceEngine::NFWindow::GetWindowHandler() const
{
    return mHwnd;
}


bool NineForceEngine::NFWindow::Init()
{
    if (!InitWindow())
    {
        return false;
    }

    mIns = this;

    mHasInit = true;

    return true;
}


void NineForceEngine::NFWindow::OnResize()
{
    
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


LRESULT NineForceEngine::NFWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_DESTROY:
        case WM_CLOSE:
        {
            PostQuitMessage(0);
            return 0;
        }
        case WM_SIZE:
        {
            if (mIns == nullptr || !mIns->mHasInit)
            {
                return 0;
            }

            const int _clientWidth = LOWORD(lParam);

            const int _clientHeight = HIWORD(lParam);

            NFGlobalConfig::Instance()->SetResolutionWidth(_clientWidth);

            NFGlobalConfig::Instance()->SetResolutionHeight(_clientHeight);

            if (wParam == SIZE_MINIMIZED)
            {
            }
            else if (wParam == SIZE_MAXIMIZED)
            {
                mIns->OnResize();
            }
            else if (wParam == SIZE_RESTORED)
            {
                mIns->OnResize();
            }

            return 0;
        }

        default:
        {
            return MessageHandler(hWnd, message, wParam, lParam);
        }
    }
}


bool NineForceEngine::NFWindow::InitWindow()
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
    _wc.style = CS_HREDRAW | CS_VREDRAW;
    _wc.lpfnWndProc = WndProc;
    _wc.cbClsExtra = 0;
    _wc.cbWndExtra = 0;
    _wc.hInstance = mHandleInstance;
    _wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    _wc.hIconSm = _wc.hIcon;
    _wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    _wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(GRAY_BRUSH));
    _wc.lpszMenuName = nullptr;
    _wc.lpszClassName = mApplicationName;
    _wc.cbSize = sizeof(WNDCLASSEX);

    // Register the window class.
    if (!RegisterClassEx(&_wc))
    {
        MessageBox(
            nullptr,
            L"Register window class failed!",
            nullptr,
            0
        );

        return false;
    }

    // Determine the resolution of the clients desktop screen.
    auto _screenWidth = GetSystemMetrics(SM_CXSCREEN);
    auto _screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // Setup the screen settings depending on whether it is running in full screen or in windowed mode.
    if (NFGlobalConfig::Instance()->GetIsFullScreen())
    {
        // If full screen set the screen to maximum size of the users desktop and 32bit.
        memset(&_dmScreenSettings, 0, sizeof(_dmScreenSettings));
        _dmScreenSettings.dmSize = sizeof(_dmScreenSettings);
        _dmScreenSettings.dmPelsWidth = static_cast<unsigned long>(_screenWidth);
        _dmScreenSettings.dmPelsHeight = static_cast<unsigned long>(_screenHeight);
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
        _screenWidth = NFGlobalConfig::Instance()->GetResolutionWidth();
        _screenHeight = NFGlobalConfig::Instance()->GetResolutionHeight();

        // Place the window in the middle of the screen.
        _posX = (GetSystemMetrics(SM_CXSCREEN) - _screenWidth) / 2;
        _posY = (GetSystemMetrics(SM_CYSCREEN) - _screenHeight) / 2;
    }

    // Create the window with the screen settings and get the handle to it.
    mHwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        mApplicationName,
        mApplicationName,
        // this options are try to block min & max & resize by mouse drag
        WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX ^ WS_MINIMIZEBOX,
        _posX,
        _posY,
        _screenWidth,
        _screenHeight,
        nullptr,
        nullptr,
        mHandleInstance,
        nullptr
    );

    // Bring the window up on the screen and set it as main focus.
    ShowWindow(mHwnd, SW_SHOW);

    UpdateWindow(mHwnd);

    SetForegroundWindow(mHwnd);

    SetFocus(mHwnd);

    return true;
}
