#include <windows.h>
#include <stdio.h>
#include <stdint.h>

typedef int (*StartFunctionType)(void (*callback)(int32_t), uint32_t);

typedef int (*StopFunctionType)(void);

void myCallback(int32_t randomNum) {
    printf("Recv Random Value: %d\n", randomNum);
}

int main() {
    HINSTANCE hDll;
    StartFunctionType start;
    StopFunctionType stop;

    hDll = LoadLibrary("cbLib.dll");
    if (hDll == NULL) {
        printf("Load DLL Fail\n");
        return -1;
    }

    start = (StartFunctionType) GetProcAddress(hDll, "start");
    if (start == NULL) {
        printf("Load start Function Fail\n");
        FreeLibrary(hDll);
        return -1;
    }

    stop = (StopFunctionType) GetProcAddress(hDll, "stop");
    if (stop == NULL) {
        printf("Load stop Function Fail\n");
        FreeLibrary(hDll);
        return -1;
    }

    if (start(myCallback, 100) == 0) {
        printf("Thread Start Success\n");
        Sleep(3000);
        if (stop() == 0) {
            printf("Thread Stop Success\n");
        } else {
            printf("Thread Stop Fail\n");
        }
    } else {
        printf("Thread Start Fail\n");
    }

    FreeLibrary(hDll);

    return 0;
}
