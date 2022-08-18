/**
 * @file monitor-wrapper-open.c
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

#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int open(const char * pathname, int oflag, ...)
{
    int (*original)(const char *, int, ...);
    original = ORIGINAL_FUNCTION("open");
    va_list argp;
    va_start(argp, oflag);
    int fd = (*original)(pathname, oflag, argp);
    if(oflag & O_CREAT)
    {
        int mode = va_arg(argp, int);
        on_event("open", (const char *[]){STRING_SERIALIZE(pathname),
                                          OPEN_FLAGS_SERIALIZE(oflag), 
                                          MODE_FLAGS_SERIALIZE(mode),
                                          INT_SERIALIZE(fd),
                                          NULL});
    }
    else
    {
        on_event("open", (const char *[]){STRING_SERIALIZE(pathname),
                                          OPEN_FLAGS_SERIALIZE(oflag),
                                          INT_SERIALIZE(fd),
                                          NULL});
    }
    va_end(argp);
    return fd;
}

int close(int fd)
{
    int (*original)(int);
    original = ORIGINAL_FUNCTION("close");
    int result = (*original)(fd);
    on_event("close", (const char *[]){INT_SERIALIZE(fd),
                                       INT_SERIALIZE(result),
                                       NULL});
    return result;
}

FILE * fopen(const char *pathname, const char * mode)
{
    FILE * (*original)(const char *, const char *);
    original = ORIGINAL_FUNCTION("fopen");
    FILE * result = (*original)(pathname, mode);
    int fd = -1;
    if(result != NULL)
    {
        fd = fileno(result);
    }
    on_event("fopen", (const char *[]){STRING_SERIALIZE(pathname),
                                       OPEN_FLAGS_STRING_SERIALIZE(mode),
                                       NONE_NULL_SERIALIZE(result),
                                       INT_SERIALIZE(fd),
                                       NULL});
    return result;
}

int fclose(FILE * f)
{
    int (*original)(FILE*);
    original = ORIGINAL_FUNCTION("fclose");
    int fd = -1;
    if(f != NULL)
    {
        fd = fileno(f);
    }
    int result = (*original)(f);
    on_event("fclose", (const char *[]){INT_SERIALIZE(fd), 
                                        INT_SERIALIZE(result),
                                        NULL});
    return result;
}
