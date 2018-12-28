// PSMTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "PSMTest.h"

#define MAX_LOADSTRING 100

typedef INT
(WINAPI* LPKPSM)(
    HDC hdc,
    int x,
    int y,
    LPCWSTR lpString,
    int cString,
    DWORD dwFlags
);
HINSTANCE hLpk = NULL;
LPKPSM LpkPSMTextOut = NULL;
BOOL isLoaded = FALSE;

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// LpkPSMTextOut(HDC hdc, int x, int y, LPCWSTR lpString, int cString, DWORD dwFlags)

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
BOOL                LoadFunction();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.
    isLoaded = LoadFunction();

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PSMTEST, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PSMTEST));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PSMTEST));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PSMTEST);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
    {
        enum {
            ALEF = 0x5D0,
            BET,
            GIMEL,
            DALET,
            HEY,
            VAV,
            ZAYIN,
            HET,
            TET,
            YUD,
            KAF_SOFIT,
            KAF,
            LAMED,
            MEM_SOFIT,
            MEM,
            NUN_SOFIT,
            NUN,
            SAMEKH,
            AYIN,
            PEY_SOFIT,
            PEY,
            TSADI_SOFIT,
            TSADI,
            QOF,
            RESH,
            SHIN,
            TAV
            };

            LOGFONTW font;
            HFONT hfont;
            WCHAR Test[] = L"&Test Text";
            WCHAR Test2[] = L"&Test\nText";
            WCHAR Test3[] = L"&Test\tText";
            WCHAR Test4[] = L"&Test T&ex&t";
            WCHAR Test5[] = { TET, QOF, SAMEKH, TET, L' ', L'&', BET, DALET, YUD, QOF, HEY, 0};
            WCHAR Test6[] = { TET, QOF, SAMEKH, TET, L' ', BET, DALET, YUD, QOF, HEY, 0 };
            WCHAR Result[100];
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            UINT ret;
            // TODO: Add any drawing code that uses hdc here...
            ZeroMemory(&font, sizeof(LOGFONTW));
            wcscpy_s(font.lfFaceName, 32, L"Microsoft Sans Serif");
            hfont = CreateFontIndirectW(&font);
            SelectObject(hdc, hfont);

            ret = LpkPSMTextOut(hdc, 0, 0, Test, 11, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 0, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 20, Test, 11, DT_NOPREFIX);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 20, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 40, Test, 11, DT_PREFIXONLY);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 40, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 60, Test2, 11, DT_SINGLELINE);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 60, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 80, Test2, 11, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 80, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 100, Test2, 11, DT_CALCRECT);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 100, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 120, Test3, 11, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 120, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 140, Test3, 11, DT_EXPANDTABS);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 140, Result, wcslen(Result));
            //ret = LpkPSMTextOut(hdc, 0, 160, NULL, 11, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 160, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 180, Test4, 13, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 180, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 200, Test4, 13, DT_NOPREFIX);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 200, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 220, Test4, 13, DT_HIDEPREFIX);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 220, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 240, Test5, 12, 0);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 240, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 260, Test5, 12, DT_NOPREFIX);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 260, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 280, Test5, 12, DT_HIDEPREFIX);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 280, Result, wcslen(Result));
            ret = LpkPSMTextOut(hdc, 0, 300, Test5, 12, DT_PREFIXONLY);
            StringCchPrintfW(Result, 100, L"Return Value = %u", ret);
            TextOutW(hdc, 200, 300, Result, wcslen(Result));
            ExtTextOutW(hdc, 0, 320,ETO_IGNORELANGUAGE, NULL, Test6, 11, NULL);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

BOOL LoadFunction()
{
    hLpk = LoadLibraryW(L"user32.dll");

    if (!hLpk)
        return FALSE;

    LpkPSMTextOut = (LPKPSM)GetProcAddress(hLpk, "UserLpkPSMTextOut");

    if (!LpkPSMTextOut)
        return FALSE;

    return TRUE;
}
