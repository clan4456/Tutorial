#include "library.h"

#include <stdio.h>
#include <windows.h>

#define DOUBLE_SIZE sizeof(double)
#define INT32_SIZE sizeof(int32_t)
#define LV_EPOCH_DIFF 2082844800.0
#define LOG_FILE_NAME "LVUserEventWrap.log"

static cbFunc callback_func = NULL;
static LVUserEventRef *event_ref_uint8arr = NULL;
static LVUserEventRef *event_ref_int32val = NULL;
static FILE *log_file = NULL;

void getLabVIEWTime(double *lvTime, char *timeStrLocal, size_t strSize) {
    FILETIME ft, ftLocal;
    SYSTEMTIME stLocal;

    GetSystemTimeAsFileTime(&ft);
    FileTimeToLocalFileTime(&ft, &ftLocal);
    FileTimeToSystemTime(&ftLocal, &stLocal);

    sprintf_s(timeStrLocal, strSize, "%04d-%02d-%02d %02d:%02d:%02d.%03d",
              stLocal.wYear, stLocal.wMonth, stLocal.wDay,
              stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);

    ULARGE_INTEGER ull;
    ull.LowPart = ft.dwLowDateTime;
    ull.HighPart = ft.dwHighDateTime;

    *lvTime = ((ull.QuadPart - 116444736000000000ULL) / 10000000.0) + LV_EPOCH_DIFF;
}


void callbackFunc(int32_t randomNum) {
    char timeStr[30];
    double seconds;
    getLabVIEWTime(&seconds, timeStr, sizeof(timeStr));

    if (event_ref_uint8arr != NULL && event_ref_int32val != NULL) {
        static uint8_t output[DOUBLE_SIZE + INT32_SIZE];
        uint8_t *ptr_time = (uint8_t *) &seconds;
        uint8_t *ptr_num = (uint8_t *) &randomNum;
        for (int i = 0; i < INT32_SIZE; i++) {
            output[i] = ptr_num[i];
        }
        for (int i = 0; i < DOUBLE_SIZE; i++) {
            output[i + INT32_SIZE] = ptr_time[i];
        }

        LStrHandle lvDataHandle = (LStrHandle) DSNewHandle(
            sizeof(int32) + (DOUBLE_SIZE + INT32_SIZE) * sizeof(uChar));
        memset(LStrBuf(*lvDataHandle), '\0',DOUBLE_SIZE + INT32_SIZE);
        for (int i = 0; i < DOUBLE_SIZE + INT32_SIZE; i++) {
            LStrBuf(*lvDataHandle)[i] = (uChar) output[i];
        }
        LStrLen(*lvDataHandle) = DOUBLE_SIZE + INT32_SIZE;
        int32_t lvErr_uint8arr = PostLVUserEvent(*event_ref_uint8arr, &lvDataHandle);
        int32_t lvErr_int32val = PostLVUserEvent(*event_ref_int32val, &randomNum);
        errno_t err = fopen_s(&log_file, LOG_FILE_NAME, "a");
        if (err == 0) {
            fprintf(log_file, "[%s] callbackFunc: %3d, LVErrNum(Uint8Array): %d, LVErrNum(Int32Value): %d\n",
                    timeStr, randomNum, lvErr_uint8arr, lvErr_int32val);
            fclose(log_file);
        }
    }
}

cbFunc getFuncPtr(LVUserEventRef *eventRef_uint8arr, LVUserEventRef *eventRef_int32val) {
    event_ref_uint8arr = eventRef_uint8arr;
    event_ref_int32val = eventRef_int32val;
    callback_func = callbackFunc;
    errno_t err = fopen_s(&log_file, LOG_FILE_NAME, "a");
    if (err == 0) {
        fprintf(log_file,
                "GET Func Ptr UserEventRef(Uint8Array): %08X, UserEventRef(Int32Value): %08X, callback: %p\n",
                *event_ref_uint8arr, *event_ref_int32val, callback_func);
        fclose(log_file);
    }
    return callback_func;
}
