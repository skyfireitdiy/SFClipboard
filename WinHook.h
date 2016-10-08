#ifndef WINHOOK_H
#define WINHOOK_H

#include <windows.h>
LRESULT CALLBACK keyProc(int,WPARAM,LPARAM lParam );
void unHook();
void setHook();
#endif // WINHOOK_H

