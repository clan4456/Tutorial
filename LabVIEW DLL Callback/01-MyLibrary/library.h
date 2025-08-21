#ifndef INC_01_MYLIBRARY_LIBRARY_H
#define INC_01_MYLIBRARY_LIBRARY_H

#include <stdint.h>

#define MYLIBRARY_API __declspec(dllexport)

/**
 * @brief 回调函数，循环间隔时间调用
 * @param randomNum 随机数
 */
typedef void (*cbFunc)(int32_t randomNum);

/**
 * @brief 开始运行，调用函数时传入一个回调函数，调用后自动定期地调用回调函数，
 * 并在回调函数的randomNum中返回一个随机数
 * @param callback 回调函数指针
 * @param interval_ms 调用回调函数的间隔时间，单位为毫秒
 * @return 0 if successful, -1 if failed
 */
MYLIBRARY_API int32_t start(cbFunc callback,  uint32_t interval_ms);

/**
 * @brief 停止运行，调用函数后停止运行，不再调用回调函数
 * @return 0 if successful, -1 if failed
 */
MYLIBRARY_API int32_t stop();

#endif //INC_01_MYLIBRARY_LIBRARY_H
