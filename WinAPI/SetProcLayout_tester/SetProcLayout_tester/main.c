#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    DWORD i = 0;
    DWORD target = 100;
    DWORD result = 0;
    BOOL setresult;
    BOOL getresult;

    for (;i <= target; i++)
    {
        setresult = SetProcessDefaultLayout(i);
        getresult = GetProcessDefaultLayout(&result);

        printf("SetProcessDefaultLayout %s with %u as value passed, ", setresult ? "succeeded" : "failed", i);
        printf("GetProcessDefaultLayout %s and returned %u\n", getresult ? "succeeded" : "failed", result);
    }    
    _wsystem(L"pause");

    return 1;
}