/**
 * @file monitor-wrapper-exec.c
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

#include <stdarg.h>
#include <string.h>

int execl(const char *pathname, const char *arg, ...)
{
    int (*original)(const char *, const char *, ...);
    original = ORIGINAL_FUNCTION("execl");
    va_list argp;
    va_start(argp, arg);
    on_event("execl", (const char *[]){STRING_SERIALIZE(pathname),
                                       VAR_ARGS_CONCAT_STR_SERIALIZE(arg, argp, "args"),
                                       NULL});
    return (*original)(pathname, arg, argp);
}

int execlp(const char *pathname, const char *arg, ...)
{
    int (*original)(const char *, const char *, ...);
    original = ORIGINAL_FUNCTION("execlp");
    va_list argp;
    va_start(argp, arg);
    on_event("execlp", (const char *[]){STRING_SERIALIZE(pathname),
                                        VAR_ARGS_CONCAT_STR_SERIALIZE(arg, argp, "args"),
                                        NULL});
    return (*original)(pathname, arg, argp);
}

int execle(const char *pathname, const char *arg, ...)
{
    int (*original)(const char *, const char *, ...);
    original = ORIGINAL_FUNCTION("execle");
    va_list argp;
    va_start(argp, arg);
    on_event("execle", (const char *[]){STRING_SERIALIZE(pathname),
                                        VAR_ARGS_CONCAT_STR_SERIALIZE(arg, argp, "args"),
                                        NULL});
    return (*original)(pathname, arg, argp);
}

int execv(const char *pathname, char *const argv[])
{
    int (*original)(const char *,  char * const []);
    original = ORIGINAL_FUNCTION("execv");
    on_event("execv", (const char *[]){STRING_SERIALIZE(pathname),
                                       ARGV_CONCAT_STR_SERIALIZE(argv, "args"),
                                       NULL});
    return (*original)(pathname, argv);
}

int execvp(const char *pathname, char *const argv[])
{
    int (*original)(const char *, char * const []);
    original = ORIGINAL_FUNCTION("execvp");
    on_event("execvp", (const char *[]){STRING_SERIALIZE(pathname),
                                        ARGV_CONCAT_STR_SERIALIZE(argv, "args"),
                                        NULL});
    return (*original)(pathname, argv);
}

int execvpe(const char *pathname, char *const argv[], char *const envp[])
{
    int (*original)(const char *, char * const [], char * const []);
    original = ORIGINAL_FUNCTION("execvpe");
    on_event("execvpe", (const char *[]){STRING_SERIALIZE(pathname),
                                         ARGV_CONCAT_STR_SERIALIZE(argv, "args"),
                                         ARGV_CONCAT_STR_SERIALIZE(envp, "env"),
                                         NULL});
    return (*original)(pathname, argv, envp);
}
