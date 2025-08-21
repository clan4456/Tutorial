#include "library.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

static HANDLE thread_handle = NULL;
static DWORD thread_id = 0;
static volatile int is_running = 0;
static cbFunc callbackFunc = NULL;
static uint32_t cb_interval_ms = 0;

static DWORD WINAPI thread_function(LPVOID arg) {
    while (is_running) {
        int32_t random_num = rand() % 100;
        if (callbackFunc) {
            callbackFunc(random_num);
        }
        Sleep(cb_interval_ms);
    }
    return 0;
}

int32_t start(cbFunc func, uint32_t interval_ms) {
    if (is_running) {
        return -1;
    }

    if (interval_ms < 10 || interval_ms > 10000) {
        return -1;
    }
    cb_interval_ms = interval_ms;
    callbackFunc = func;
    srand((unsigned int) time(NULL));
    is_running = 1;

    thread_handle = CreateThread(NULL, 0, thread_function, NULL, 0, &thread_id);

    if (thread_handle == NULL) {
        is_running = 0;
        return -1;
    }
    return 0;
}

int32_t stop() {
    if (!is_running) {
        return -1;
    }
    is_running = 0;

    if (WaitForSingleObject(thread_handle, INFINITE) != WAIT_OBJECT_0) {
        CloseHandle(thread_handle);
        thread_handle = NULL;
        return -1;
    }

    CloseHandle(thread_handle);
    thread_handle = NULL;
    return 0;
}
