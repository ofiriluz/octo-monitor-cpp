/**
 * @file monitor-wrapper-stat.c
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

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <errno.h>

int stat(const char *pathname, struct stat *statbuf)
{
    int (*original)(const char *, struct stat *);
    original = ORIGINAL_FUNCTION("stat");
    int return_code = original(pathname, statbuf);
    on_event("stat", (const char *[]){STRING_SERIALIZE(pathname), 
                                      STAT_SERIALIZE(statbuf), 
                                      INT_SERIALIZE(return_code),
                                      NULL});
    return return_code;
}

int fstat(int fd, struct stat *statbuf)
{
    int (*original)(int, struct stat *);
    original = ORIGINAL_FUNCTION("fstat");
    int return_code = original(fd, statbuf);
    on_event("stat", (const char *[]){FD_SERIALIZE(fd), 
                                      STAT_SERIALIZE(statbuf), 
                                      INT_SERIALIZE(return_code),
                                      NULL});
    return return_code;
}

int lstat(const char *pathname, struct stat *statbuf)
{
    int (*original)(const char *, struct stat *);
    original = ORIGINAL_FUNCTION("lstat");
    int return_code = original(pathname, statbuf);
    on_event("lstat", (const char *[]){STRING_SERIALIZE(pathname), 
                                       STAT_SERIALIZE(statbuf), 
                                       INT_SERIALIZE(return_code),
                                       NULL});
    return return_code;
}

int __xstat(int ver, const char * pathname, struct stat * statbuf)
{
    int (*original)(int, const char *, struct stat *);
    original = ORIGINAL_FUNCTION("__xstat");
    int return_code = (*original)(ver, pathname, statbuf);
    on_event("__xstat", (const char *[]){STRING_SERIALIZE(pathname), 
                                         STAT_SERIALIZE(statbuf),
                                         INT_SERIALIZE(return_code), 
                                         NULL});
    return return_code;
}

int __lxstat(int ver, const char * pathname, struct stat * statbuf)
{
    int (*original)(int, const char *, struct stat *);
    original = ORIGINAL_FUNCTION("__lxstat");
    int return_code = (*original)(ver, pathname, statbuf);
    on_event("__lxstat", (const char *[]){STRING_SERIALIZE(pathname), 
                                          STAT_SERIALIZE(statbuf), 
                                          INT_SERIALIZE(return_code),
                                          NULL});
    return return_code;
}

int __fxstat(int ver, int fd, struct stat * statbuf)
{
    int (*original)(int, int, struct stat *);
    original = ORIGINAL_FUNCTION("__fxstat");
    int return_code = (*original)(ver, fd, statbuf);
    on_event("__fxstat", (const char *[]){FD_SERIALIZE(fd), 
                                          STAT_SERIALIZE(statbuf), 
                                          INT_SERIALIZE(return_code),
                                          NULL});
    return return_code;
}
