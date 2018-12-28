#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <usp10.h>

#define SIZEOF(arr) (sizeof(arr)/sizeof(arr[0]))
LPCWSTR ClassName = L"ScriptStringTest";
HINSTANCE hInst;

static HRESULT DrawString(HDC hdc, LPCWSTR lpString, INT count, DWORD flags)
{
    static INT y = 0;
    SCRIPT_STRING_ANALYSIS ssa;
    HRESULT hr;
    LPWSTR flagstr;

    hr = ScriptStringAnalyse(hdc, lpString, count, (3 * count / 2 + 16), -1, flags, 0, NULL, NULL, NULL, NULL, NULL, &ssa);

    if (!SUCCEEDED(hr))
        return hr;

    hr = ScriptStringOut(ssa, 0, y, 0, NULL, 0, 0, FALSE);

    switch (flags ^ SSA_GLYPHS)
    {
        case SSA_HOTKEY:
            flagstr = L"SSA_HOTKEY";
            break;
        case SSA_HIDEHOTKEY:
            flagstr = L"SSA_HIDEHOTKEY";
            break;
        case SSA_HOTKEYONLY:
            flagstr = L"SSA_HOTKEYONLY";
            break;
        default:
            flagstr = L"";
            break;
    }
    TextOutW(hdc, 130, y, flagstr, wcslen(flagstr));   
    y += 20;
    ScriptStringFree(&ssa);
    return hr;
}

static VOID PaintWnd(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static LPCWSTR tests[] = { 
        L"1. Test Text",
        L"2. Test &Text",
        L"3. &Test &Text",
        L"4. Test &&Text",
        L"5. &&Test &&Text",
        L"6. Test &&&Text",
        L"7. Test &&&&Text",
        L"8. Test Text&" };
    static const DWORD flags[] = { 
        SSA_GLYPHS,
        SSA_GLYPHS | SSA_HOTKEY,
        SSA_GLYPHS | SSA_HIDEHOTKEY,
        SSA_GLYPHS | SSA_HOTKEYONLY };
    PAINTSTRUCT ps;
    HFONT hfont;
    HDC hdc;
    INT i, j;

    hdc = BeginPaint(hwnd, &ps);

    hfont = CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Sans Serif");

    if (hfont)
        SelectObject(hdc, hfont);

    for (i = 0; i < SIZEOF(tests); i++)
    {
        for (j = 0; j < SIZEOF(flags); j++)
            DrawString(hdc, tests[i], wcslen(tests[i]), flags[j]);
    }

    EndPaint(hwnd, &ps);
}
static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        PaintWnd(hwnd, uMsg, wParam, lParam);
        return TRUE;

    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return FALSE;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    HWND MainWnd;
    WNDCLASSEXW wc;
    MSG msg;

    hInst = hInstance;

    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc = MainWndProc;
    wc.hInstance = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = ClassName;
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassExW(&wc))
        return 0;

    MainWnd = CreateWindowExW(0L,
        ClassName,
        L"ScriptStringTest",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        400,
        1000,
        NULL,
        NULL,
        hInst,
        NULL);

    if (!MainWnd)
        return 0;

    ShowWindow(MainWnd, SW_SHOW);
    UpdateWindow(MainWnd);

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnregisterClassW(ClassName, hInst);
    return 0;
}
