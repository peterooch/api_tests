#include <stdio.h>
#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef INT (WINAPI* LPKINIT)(UINT InitParam);

static BOOL TestValue(UINT val)
{
    HANDLE hLPK = NULL;
    LPKINIT LpkInitialize = NULL;
    INT iResult;

    hLPK = LoadLibraryW(L"lpk.dll");

    if (!hLPK)
        return FALSE;

    LpkInitialize = (LPKINIT)GetProcAddress(hLPK, "LpkInitialize");

    if (!LpkInitialize)
    {
        FreeLibrary(hLPK);
        return FALSE;
    }

    iResult = LpkInitialize(val);

    //Tends to return always TRUE (1)
    if (iResult != TRUE)
        wprintf(L"LpkInitialize(%u) gave return of %d\n", val, iResult);

    FreeLibrary(hLPK);

    return TRUE;
}

int main()
{
    UINT start, end;

    scanf_s("%u %u", &start, &end);

    while (start <= end)
    {
        _wsystem(L"cls");
        wprintf(L"%u", start);
        TestValue(start++);
    }

    wprintf(L"%s", L"Done\n");

    getchar();
    getchar();

    return TRUE;
}