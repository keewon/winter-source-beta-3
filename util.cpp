// 각종 유용한 함수들
#include "util.h"
#include <windows.h>
void Wait(int how_long) {

    DWORD tick = timeGetTime();//GetTickCount();
    DWORD end_tick = tick + how_long;

    while (tick < end_tick) {
	Sleep(0);
	tick = timeGetTime();//GetTickCount();
    }
}