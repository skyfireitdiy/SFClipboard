#ifdef _WIN32
#include <WinHook.h>
#include <Windows.h>
#include <MainWidget.h>
#include <QDebug>

static HHOOK keyHook=NULL;
extern MainWidget* pMainWidget;

LRESULT CALLBACK keyProc(int,WPARAM wParam,LPARAM lParam)
{
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;

    if(wParam!=WM_KEYDOWN&&wParam!=WM_SYSKEYDOWN){
        return 0;
    }

    if(GetAsyncKeyState(VK_MENU)<0&&pkbhs->vkCode=='C'){
        emit pMainWidget->clear_all_sgn();
        return 1;
    }else if(GetAsyncKeyState(VK_MENU)<0&&pkbhs->vkCode=='S'){
        emit pMainWidget->frame_in_out_sgn();
        return 1;
    }else if(GetAsyncKeyState(VK_MENU)<0&&pkbhs->vkCode=='F'){
        emit pMainWidget->hide_btn->clicked();
    }

    int num=0;
    switch (pkbhs->vkCode) {
    case VK_NUMPAD0:
    case 0x30:
        num=0;
        break;
    case VK_NUMPAD1:
    case 0x31:
        num=1;
        break;
    case VK_NUMPAD2:
    case 0x32:
        num=2;
        break;
    case VK_NUMPAD3:
    case 0x33:
        num=3;
        break;
    case VK_NUMPAD4:
    case 0x34:
        num=4;
        break;
    case VK_NUMPAD5:
    case 0x35:
        num=5;
        break;
    case VK_NUMPAD6:
    case 0x36:
        num=6;
        break;
    case VK_NUMPAD7:
    case 0x37:
        num=7;
        break;
    case VK_NUMPAD8:
    case 0x38:
        num=8;
        break;
    case VK_NUMPAD9:
    case 0x39:
        num=9;
        break;
    default:
        return 0;
    }
    if(GetAsyncKeyState(VK_CONTROL)<0){
        pMainWidget->on_hot_copy(num);
        return 1;
    }else if(GetAsyncKeyState(VK_MENU)<0){
        pMainWidget->on_hot_delete(num);
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

