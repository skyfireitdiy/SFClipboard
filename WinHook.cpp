#ifdef _WIN32
#include <WinHook.h>
#include <Windows.h>
#include <MainWidget.h>
#include <QDebug>

static HHOOK keyHook=NULL;
extern MainWidget* pMainWidget;

LRESULT CALLBACK keyProc(int,WPARAM,LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;
    if(GetAsyncKeyState(VK_CONTROL)<0){
        int num=0;
        switch (pkbhs->vkCode) {
        case VK_NUMPAD0:
            num=0;
            break;
        case VK_NUMPAD1:
            num=1;
            break;
        case VK_NUMPAD2:
            num=2;
            break;
        case VK_NUMPAD3:
            num=3;
            break;
        case VK_NUMPAD4:
            num=4;
            break;
        case VK_NUMPAD5:
            num=5;
            break;
        case VK_NUMPAD6:
            num=6;
            break;
        case VK_NUMPAD7:
            num=7;
            break;
        case VK_NUMPAD8:
            num=8;
            break;
        case VK_NUMPAD9:
            num=9;
            break;
        default:
            return 0;
        }
        pMainWidget->on_hot_key(num);
        return 1;
    }
    return 0;
}

void unHook()
{
    UnhookWindowsHookEx(keyHook);
}

void setHook()
{
    keyHook =SetWindowsHookEx( WH_KEYBOARD_LL,keyProc,GetModuleHandle(NULL),0);
}

#endif

