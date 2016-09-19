#include <Windows.h>

#include <assert.h>
#include <memory>
#include <string>

#include "../GraphicsSystem/GraphicsSystem.h"
#include "../GraphicsSystem/GraphicsResource.h"

static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    WNDCLASSEX wcx{};
    wcx.cbSize = sizeof(wcx);
    wcx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wcx.hInstance = instance;
    wcx.lpfnWndProc = WindowProc;
    wcx.lpszClassName = L"GDKTestWindowClass";
    if (RegisterClassEx(&wcx) == INVALID_ATOM)
    {
        assert(false);
        return -1;
    }

    DWORD style = WS_OVERLAPPEDWINDOW & ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    RECT rc{};
    rc.right = 1280;
    rc.bottom = 720;
    AdjustWindowRect(&rc, style, FALSE);

    HWND hwnd = CreateWindow(wcx.lpszClassName, L"GDKTest", style, CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, instance, nullptr);
    if (!hwnd)
    {
        assert(false);
        return -2;
    }

    GraphicsSystemCreateParams params{};
    params.Type = GraphicsSystemType::Vulkan;
#ifdef _DEBUG
    params.EnableDebug = true;
#endif
    params.Instance = instance;
    params.Window = hwnd;
    params.BackBufferWidth = 1280;
    params.BackBufferHeight = 720;

    std::shared_ptr<GraphicsSystem> graphics = CreateGraphicsSystem(params);

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg{};
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // Idle
            graphics->Present(false);
        }
    }

    graphics.reset();
    DestroyWindow(hwnd);
    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0);
        break;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}
