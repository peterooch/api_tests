#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <usp10.h>
#include <strsafe.h>

#define RUNTEST PaintWnd_lpkGetExtents
#define SIZEOF(arr) (sizeof(arr)/sizeof(arr[0]))
LPCWSTR ClassName = L"GetExtents Test";
HINSTANCE hInst;
typedef BOOL(WINAPI* LPKGTEXP)(HDC hdc, LPCWSTR lpString, INT cString, INT nMaxExtent, INT* lpnFit, INT* lpDx, SIZE* lpSize, DWORD dwUnused, INT unknown);
HMODULE hLPK = NULL, hGDI = NULL;
typedef BOOL(WINAPI* GTEXPRIV)(HDC hdc, LPCWSTR lpwsz, INT cwc, INT dxMax, INT *pcCh, PINT pdxOut, LPSIZE psize);
GTEXPRIV GetTextExtentExPointWPri = NULL;
LPKGTEXP LpkGetTextExtentExPoint = NULL;

#if 0
static HRESULT DrawString(HDC hdc, INT y, LPCWSTR lpString, INT count, DWORD flags)
{
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
    ScriptStringFree(&ssa);
    return hr;
}

static VOID PaintWnd_scriptstring(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static LPCWSTR tests[] = { 
        L"1. Test Text",
        L"2. Test &Text",
        L"3. &Test &Text",
        L"4. Test &&Text",
        L"5. &&Test &&Text",
        L"6. Test &&&Text",
        L"7. Test &&&&Text",
        L"8. Test Text&",
        L"9. \u05D8\u05E7\u05E1\u05D8 &\u05D1\u05D3\u05D9\u05E7\u05D4" };
    static const DWORD flags[] = { 
        SSA_GLYPHS,
        SSA_GLYPHS | SSA_HOTKEY,
        SSA_GLYPHS | SSA_HIDEHOTKEY,
        SSA_GLYPHS | SSA_HOTKEYONLY };
    PAINTSTRUCT ps;
    HFONT hfont;
    HDC hdc;
    INT i, j, y;

    hdc = BeginPaint(hwnd, &ps);

    hfont = CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Sans Serif");

    if (hfont)
        SelectObject(hdc, hfont);

    for (i = 0, y = 0; i < SIZEOF(tests); i++)
    {
        for (j = 0; j < SIZEOF(flags); j++, y += 20)
            DrawString(hdc, y, tests[i], wcslen(tests[i]), flags[j]);
    }
    DrawString(hdc, y, tests[8], wcslen(tests[8]), flags[1] | SSA_RTL);

    EndPaint(hwnd, &ps);
}
#endif
static VOID PaintWnd_lpkGetExtents(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    SIZE sz;
    WCHAR buffer[MAX_PATH] = { 0 };
    INT fit = 0, dx[10], i, j = 0;
    UINT  buffer_length; 
    HFONT hFont;
    WCHAR hebrew_txt1[20];
    WCHAR hebrew_txt2[20];
    WCHAR arabic_txt[20];
    GCP_RESULTSW gcp_res;
            WCHAR glyphs[10];
            SCRIPT_STRING_ANALYSIS ssa;
            BOOL result;

    if (!hLPK)
    {
        hGDI = LoadLibraryW(L"gdi32.dll");
        hLPK = LoadLibraryW(L"lpk.dll");
        if (!hLPK) return;

        LpkGetTextExtentExPoint = (LPKGTEXP)GetProcAddress(hLPK, "LpkGetTextExtentExPoint");
        GetTextExtentExPointWPri = (GTEXPRIV)GetProcAddress(hGDI, "GetTextExtentExPointWPri");
        if (!LpkGetTextExtentExPoint)
        {
            FreeLibrary(hLPK);
            return;
        }
    }
    LoadStringW(hInst, 1, hebrew_txt1, 20);
    LoadStringW(hInst, 2, hebrew_txt2, 20);
    LoadStringW(hInst, 3, arabic_txt, 20);
            hFont = CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, L"Microsoft Sans Serif");
            SelectObject(hdc, hFont);

            LpkGetTextExtentExPoint(hdc, L"Dummy text", 10, 0, &fit, NULL, &sz, 0, 0);
            StringCchPrintfW(buffer, MAX_PATH, L"LPK: For \"Dummy Text\" sz.x = %d, sz.y = %d, lpnFit = %d", sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 10, buffer, buffer_length);
            GetTextExtentExPointW(hdc, L"Dummy text", 10, 0, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"NOT LPK: For \"Dummy Text\" sz.x = %d, sz.y = %d, lpnFit = %d", sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 30, buffer, buffer_length);
            LpkGetTextExtentExPoint(hdc, hebrew_txt1, 7, 0, &fit, NULL, &sz, 0, 0);
            StringCchPrintfW(buffer, MAX_PATH, L"LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",hebrew_txt1, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 50, buffer, buffer_length);
            GetTextExtentExPointW(hdc, hebrew_txt1, 7, 0, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"NOT LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",hebrew_txt1, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 70, buffer, buffer_length);
            LpkGetTextExtentExPoint(hdc, hebrew_txt2, 15, 100, &fit, NULL, &sz, 0, 0);
            StringCchPrintfW(buffer, MAX_PATH, L"LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",hebrew_txt2, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 90, buffer, buffer_length);
            GetTextExtentExPointW(hdc, hebrew_txt2, 15, 100, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"NOT LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",hebrew_txt2, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 110, buffer, buffer_length);
            GetTextExtentExPointWPri(hdc, hebrew_txt2, 15, 100, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"WPri: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",hebrew_txt2, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 130, buffer, buffer_length);
            LpkGetTextExtentExPoint(hdc, arabic_txt, 9, 0, &fit, NULL, &sz, 0, 0);
            StringCchPrintfW(buffer, MAX_PATH, L"LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",arabic_txt, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 150, buffer, buffer_length);
            GetTextExtentExPointW(hdc, arabic_txt, 9, 0, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"NOT LPK: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",arabic_txt, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 170, buffer, buffer_length);
            GetTextExtentExPointWPri(hdc, arabic_txt, 9, 0, &fit, NULL, &sz);
            StringCchPrintfW(buffer, MAX_PATH, L"WPri: For \"%s\" sz.x = %d, sz.y = %d, lpnFit = %d",arabic_txt, sz.cx, sz.cy, fit);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 190, buffer, buffer_length);
            TextOutW(hdc, 10, 210, L"Dx comparison for arabic sample", 31);
            TextOutW(hdc, 10, 230, L"LpkGetTextExtentExPoint", 23);
            ZeroMemory(dx, sizeof(dx));
            LpkGetTextExtentExPoint(hdc, arabic_txt, 9, 250, &fit, dx, &sz, 0, 0);
            for (i = 0; i < 10; i++)
            {
                StringCchPrintfW(buffer, MAX_PATH, L"%d", dx[i]);
                StringCchLengthW(buffer, MAX_PATH, &buffer_length);
                TextOutW(hdc, 10 + (i - 1) * 30, 250, buffer, buffer_length);
            }
            StringCchPrintfW(buffer, MAX_PATH, L"size.cx = %d, size.cy = %d", sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 270, buffer, buffer_length);
            TextOutW(hdc, 10, 290, L"GetTextExtentExPointW", 21);
            ZeroMemory(dx, sizeof(dx));
            GetTextExtentExPointW(hdc, arabic_txt, 9, 250, &fit, dx, &sz);
            for (i = 0; i < 10; i++)
            {
                StringCchPrintfW(buffer, MAX_PATH, L"%d", dx[i]);
                StringCchLengthW(buffer, MAX_PATH, &buffer_length);
                TextOutW(hdc, 10 + (i - 1) * 30, 310, buffer, buffer_length);
            }
            StringCchPrintfW(buffer, MAX_PATH, L"size.cx = %d, size.cy = %d", sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 330, buffer, buffer_length);
            TextOutW(hdc, 10, 350, L"GetTextExtentExPointWPri", 24);
            ZeroMemory(dx, sizeof(dx));
            GetTextExtentExPointWPri(hdc, arabic_txt, 9, 250, &fit, dx, &sz);
            for (i = 0; i < 10; i++)
            {
                StringCchPrintfW(buffer, MAX_PATH, L"%d", dx[i]);
                StringCchLengthW(buffer, MAX_PATH, &buffer_length);
                TextOutW(hdc, 10 + (i - 1) * 30, 370, buffer, buffer_length);
            }
            StringCchPrintfW(buffer, MAX_PATH, L"size.cx = %d, size.cy = %d", sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 390, buffer, buffer_length);
            ZeroMemory(dx, sizeof(dx));
            ZeroMemory(glyphs, sizeof(glyphs));
            ZeroMemory(&gcp_res, sizeof(gcp_res));
            gcp_res.lpGlyphs = glyphs;
            gcp_res.nGlyphs = 10;
            GetCharacterPlacementW(hdc, arabic_txt, 9, 0, &gcp_res, GCP_REORDER | GCP_GLYPHSHAPE);
            TextOutW(hdc, 10, 410, L"GetTextExtentExPointI with GCPW", 32);
            GetTextExtentExPointI(hdc, (LPWORD)glyphs, gcp_res.nGlyphs, 250, &fit, dx, &sz);
            for (i = 0; i < 10; i++)
            {
                StringCchPrintfW(buffer, MAX_PATH, L"%d", dx[i]);
                StringCchLengthW(buffer, MAX_PATH, &buffer_length);
                TextOutW(hdc, 10 + (i - 1) * 30, 430, buffer, buffer_length);
            }
            StringCchPrintfW(buffer, MAX_PATH, L"size.cx = %d, size.cy = %d", sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 450, buffer, buffer_length);
            TextOutW(hdc, 10, 470, L"USP10 ScriptString", 18);
            ScriptStringAnalyse(hdc, arabic_txt, 9, 1.5 * 9 + 16, -1, SSA_GLYPHS, 0, NULL, NULL, NULL, NULL, NULL, &ssa);
            ScriptStringGetLogicalWidths(ssa, dx);
            for (i = 0; i < 9; i++)
            {
                j += dx[i];
                StringCchPrintfW(buffer, MAX_PATH, L"%d", j);
                StringCchLengthW(buffer, MAX_PATH, &buffer_length);
                TextOutW(hdc, 10 + (i - 1) * 30, 490, buffer, buffer_length);
            }
            result = LpkGetTextExtentExPoint(hdc, NULL, 10, 0, NULL, NULL, &sz, 0,0);
            StringCchPrintfW(buffer, MAX_PATH, L"LpkGetTextExtentExPoint(hdc, NULL, 10, 0, NULL, NULL, &sz, 0, 0) returned %d, sz.cx = %d, sz.cy = %d",
                             result, sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 510, buffer,buffer_length);
            result = LpkGetTextExtentExPoint(hdc, arabic_txt, 0, 0, NULL, NULL, &sz, 0,0);
            StringCchPrintfW(buffer, MAX_PATH, L"LpkGetTextExtentExPoint(hdc, %s, 0, 0, NULL, NULL, &sz, 0, 0) returned %d, sz.cx = %d, sz.cy = %d",
                             arabic_txt,result, sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 530, buffer,buffer_length);
            result = LpkGetTextExtentExPoint(hdc, arabic_txt, 10, 0, NULL, NULL, NULL, 0,0);
            StringCchPrintfW(buffer, MAX_PATH, L"LpkGetTextExtentExPoint(hdc, %s, 10, 0, NULL, NULL, NULL, 0, 0) returned %d, sz.cx = %d, sz.cy = %d",
                             arabic_txt,result, sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 550, buffer,buffer_length);
            result = LpkGetTextExtentExPoint(hdc, arabic_txt, -20, 0, NULL, NULL, &sz, 0,0);
            StringCchPrintfW(buffer, MAX_PATH, L"LpkGetTextExtentExPoint(hdc, %s, -20, 0, NULL, NULL, &sz, 0, 0) returned %d, sz.cx = %d, sz.cy = %d",
                             arabic_txt,result, sz.cx, sz.cy);
            StringCchLengthW(buffer, MAX_PATH, &buffer_length);
            TextOutW(hdc, 10, 570, buffer,buffer_length);
            EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_PAINT:
        RUNTEST(hwnd, uMsg, wParam, lParam);
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
        L"GetExtents Test",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        1000,
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
