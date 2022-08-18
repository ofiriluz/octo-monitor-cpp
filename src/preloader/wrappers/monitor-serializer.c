/**
 * @file monitor-serializer.c
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>

char * serialize_int(int n)
{
    char buffer[128];
    int len = snprintf(buffer, 128, "%d", n);
    buffer[len] = '\0';
    return strdup(buffer);
}

char * serialize_long(long l)
{
    char buffer[128];
    int len = snprintf(buffer, 128, "%ld", l);
    buffer[len] = '\0';
    return strdup(buffer);
}

char * serialize_bool(int b)
{
    if(b)
    {
        return "true";
    }
    return "false";
}

char * serialize_timespec(struct timespec spec)
{
    return serialize_long(spec.tv_sec);
}

char * serialize_fd_filename(int fd)
{
    char path_buffer[256];
    snprintf(path_buffer, 256, "/proc/self/fd/%d", fd);
    size_t size = readlink(path_buffer, path_buffer, 256);
    path_buffer[size] = '\0';
    return strdup(path_buffer);
}

char * serialize_vararg_to_string_null_terminated(const char * farg, va_list argp)
{
    char * concatted_string = strdup(farg);
    const char * arg;
    while((arg = va_arg(argp, const char *)))
    {
        size_t new_size, old_size;
        if(concatted_string == NULL) 
        {
            new_size = strlen(arg) + 1;
            concatted_string = malloc(new_size);
            *concatted_string = '\0';
        }
        else
        {
            old_size = strlen(concatted_string) + 1;
            new_size = strlen(concatted_string) + strlen(arg) + 2;
            concatted_string = realloc(concatted_string, new_size);
            concatted_string[old_size-1] = ' ';
            concatted_string[old_size] = '\0';
        }
        strcat(concatted_string, arg); 
    }
    va_end(argp);
    return concatted_string;
}

char * serialize_argv_to_string(char *const argv[])
{
    int size = 0;
    int j = 0;
    const char ** ptr = (const char **)argv;
    while(*ptr)
    {
        size++;
        ptr++;
    }

    char * concatted_string = malloc(size * 1024 + size + 1);
    *concatted_string = '\0';
    for(j=0;j<size;j++)
    {
        if(strlen(concatted_string) != 0)
        {
            int old_size = strlen(concatted_string);
            concatted_string[old_size] = ' ';
            concatted_string[old_size + 1] = '\0';
        }
        strcat(concatted_string, *argv);
        argv++;
    }

    return concatted_string;
}
