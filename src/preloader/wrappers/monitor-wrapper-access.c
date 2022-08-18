/**
 * @file monitor-wrapper-access.c
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/preloader/monitor-preloader.h"
#include "octo-monitor-cpp/preloader/wrappers/monitor-wrapper.h"
#include "octo-monitor-cpp/preloader/wrappers/monitor-serializer.h"

int access(const char *pathname, int mode)
{
    int (*original)(const char *, int);
    original = ORIGINAL_FUNCTION("access");
    int result = original(pathname, mode);
    on_event("access", (const char *[]){STRING_SERIALIZE(pathname),
                                        ACCESS_MODE_SERIALIZE(mode),
                                        INT_SERIALIZE(result),
                                        NULL});

    return result;
}
