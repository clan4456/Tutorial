#ifndef INC_02_LVWRAP_LIBRARY_H
#define INC_02_LVWRAP_LIBRARY_H

#include "extcode.h"

#define LVWRAP_API __declspec(dllexport)

typedef void (*cbFunc)(int32_t randomNum);

LVWRAP_API cbFunc getFuncPtr(LVUserEventRef *eventRef_uint8arr, LVUserEventRef *eventRef_int32val);

#endif //INC_02_LVWRAP_LIBRARY_H
