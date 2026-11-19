#pragma comment(lib, "user32.lib")
#include <Windows.h>
#include <wchar.h>
#include <stdbool.h>

typedef struct
{
    wchar_t WindowName[256];
    wchar_t ClassName[256];
    
} WindowInfo;

WindowInfo* GetWindowInfoByHWND(HWND hwnd)
{
    if (!hwnd || !IsWindow(hwnd)) return NULL;

    WindowInfo* pWindowInfo = (WindowInfo*)malloc(sizeof(WindowInfo));
    if (!pWindowInfo) return NULL;

    GetClassNameW(hwnd, pWindowInfo->ClassName, sizeof(pWindowInfo->ClassName));
    GetWindowTextW(hwnd, pWindowInfo->WindowName, sizeof(pWindowInfo->WindowName));

    return pWindowInfo;
}

void mainLoop()
{
    wchar_t appName[] = L"win32windowclicker";

    while (1)
    {
        //if mouse got clicked (the only pure methods to capture key input besides writing a message loop because i am really lazy)
        if (GetAsyncKeyState(VK_RETURN))
        {
            POINT point = {0};
            HWND hDesktop = GetDesktopWindow();
            BOOL result = GetCursorPos(&point);
            if (result)
            {
                HWND hResultWindow = WindowFromPhysicalPoint(point);
                if (hResultWindow && hResultWindow != hDesktop) 
                {
                    WindowInfo* pWindowInfo = GetWindowInfoByHWND(hResultWindow);
                    if (!pWindowInfo) MessageBoxW(NULL, L"Memory allocation failed, the program will now exit", &appName, MB_ICONERROR);

                    wchar_t windowMsgBuffer[1024] = L"";
                    swprintf(&windowMsgBuffer, sizeof(windowMsgBuffer), L"The captured HWND's information is \n Window name: %ls \n Class name: %ls", pWindowInfo->WindowName, pWindowInfo->ClassName);
                    wcscat(&windowMsgBuffer, pWindowInfo->WindowName);

                    MessageBoxW(NULL, &windowMsgBuffer, &appName, MB_ICONINFORMATION);

                    free(pWindowInfo);
                }
            }
            else
            {
                MessageBoxW(NULL, L"Could not find a valid HWND", L"Win32windowClicker", MB_OK);
            }
        }

        //100ms delay so it doesnt bog down the CPU
        Sleep(100);
        
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    mainLoop();
    return 0;
}
