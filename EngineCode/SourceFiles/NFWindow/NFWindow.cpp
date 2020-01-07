#include "NFWindow/NFWindow.h"

using Microsoft::WRL::ComPtr;
using namespace std;


NFWindow::NFWindow(HINSTANCE hIns) : mIns(hIns)
{
    mNFWindow = this;
}


NFWindow* NFWindow::mNFWindow = nullptr;


LRESULT CALLBACK MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    auto _window = NFWindow::GetWindowPtr();

    if (_window == nullptr)
    {
        return -1;
    }

    // Forward hwnd on because we can get messages (e.g., WM_CREATE)
    // before CreateWindow returns, and thus before mhMainWnd is valid.
    return _window->MsgProc(hwnd, msg, wParam, lParam);
}


bool NFWindow::InitD3D() const
{
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> _debugController;

        auto _result = D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController));
        if (FAILED(_result))
        {
            MessageBox(
                nullptr,
                L"Create DirectX Debug failed!",
                L"Error",
                MB_OK
            );

            return false;
        }

        _debugController->EnableDebugLayer();
    }

#endif


    return true;
}


void NFWindow::Update()
{
}


int NFWindow::Run()
{
    MSG _msg = {nullptr};

    while (_msg.message != WM_QUIT)
    {
        if (PeekMessage(&_msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&_msg);

            DispatchMessage(&_msg);
        }
        else
        {
            Update();

            Draw();
        }
    }

    return static_cast<int>(_msg.wParam);
}


void NFWindow::Draw()
{
}


NFWindow* NFWindow::GetWindowPtr()
{
    return mNFWindow;
}


LRESULT NFWindow::MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);

            return 0;
        }
        default:
        {
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
    }
}


bool NFWindow::Init()
{
    auto _window = NFWindow::GetWindowPtr();

    if (_window == nullptr)
    {
        return false;
    }

    if (!InitWindow())
    {
        return false;
    }

    if (!InitD3D())
    {
        return false;
    }

    return true;
}


bool NFWindow::InitWindow()
{
    WNDCLASS _wc;

    _wc.style = CS_HREDRAW | CS_VREDRAW;
    _wc.cbClsExtra = 0;
    _wc.cbWndExtra = 0;
    _wc.lpfnWndProc = MainWndProc;
    _wc.hInstance = mIns;
    _wc.hIcon = LoadIcon(0, IDI_APPLICATION);
    _wc.hCursor = LoadCursor(0, IDC_ARROW);
    _wc.hbrBackground = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));
    _wc.lpszMenuName = 0;
    _wc.lpszClassName = L"NFWindow";

    if (!RegisterClass(&_wc))
    {
        MessageBox(0, L"Register windows class failed!", 0, 0);

        return false;
    }

    RECT _r = {0, 0, mClientWidth, mClientHeight};

    AdjustWindowRect(&_r, WS_OVERLAPPEDWINDOW, false);

    const auto _width = _r.right - _r.left;

    const auto _height = _r.bottom - _r.top;

    mWnd = CreateWindow(
        L"NFWindow",
        mMainWndCaption.c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        _width,
        _height,
        nullptr,
        nullptr,
        mIns,
        nullptr
    );

    if (mWnd == nullptr)
    {
        MessageBox(nullptr, L"Create window failed!", nullptr, 0);

        return false;
    }

    ShowWindow(mWnd, SW_SHOW);

    UpdateWindow(mWnd);

    return true;
}
