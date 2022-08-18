/**
 * @file monitor-serializer.h
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_SERIALIZER_HPP_
#define MONITOR_SERIALIZER_HPP_

#include <time.h>
#include <stdint.h>
#include <unistd.h>

#define STRING_SERIALIZE(NAME) \
    #NAME, NAME

#define INT_SERIALIZE(NAME) \
    #NAME, serialize_int(NAME)

#define NONE_NULL_SERIALIZE(NAME) \
    #NAME, serialize_bool(NAME != NULL)

#define FD_SERIALIZE(NAME) \
    "fd", serialize_int(NAME), \
    "fd_filename", serialize_fd_filename(NAME)

#ifdef __APPLE__
#define STAT_SERIALIZE(NAME) \
    "stat.mode", serialize_int(NAME->st_mode), \
    "stat.uid", serialize_int(NAME->st_uid), \
    "stat.gid", serialize_int(NAME->st_gid), \
    "stat.size", serialize_int(NAME->st_size), \
    "stat.access_time", serialize_timespec(NAME->st_atimespec), \
    "stat.modify_time", serialize_timespec(NAME->st_mtimespec)
#else
#define STAT_SERIALIZE(NAME) \
    "stat.mode", serialize_int(NAME->st_mode), \
    "stat.uid", serialize_int(NAME->st_uid), \
    "stat.gid", serialize_int(NAME->st_gid), \
    "stat.size", serialize_int(NAME->st_size), \
    "stat.access_time", serialize_timespec(NAME->st_atim), \
    "stat.modify_time", serialize_timespec(NAME->st_mtim)
#endif

#define OPEN_FLAGS_SERIALIZE(NAME) \
    "open.read", serialize_bool(NAME == O_RDONLY || NAME & O_RDWR), \
    "open.write", serialize_bool(NAME & O_WRONLY || NAME & O_RDWR), \
    "open.append", serialize_bool(NAME & O_APPEND), \
    "open.truncate", serialize_bool(NAME & O_TRUNC), \
    "open.create", serialize_bool(NAME & O_CREAT)

#define OPEN_FLAGS_STRING_SERIALIZE(NAME) \
    "open.read", serialize_bool(strchr(NAME, 'r') != NULL || strchr(NAME, '+') != NULL), \
    "open.write", serialize_bool(strchr(NAME, 'w') != NULL || strchr(NAME, '+') != NULL || strchr(NAME, 'a') != NULL), \
    "open.append", serialize_bool(strchr(NAME, 'a') != NULL), \
    "open.truncate", serialize_bool(strchr(NAME, 'w') != NULL || strchr(NAME, '+') != NULL), \
    "open.create", serialize_bool(strchr(NAME, 'w') != NULL || strchr(NAME, 'a') != NULL)

#define MODE_FLAGS_SERIALIZE(NAME) \
    "mode.user.read", serialize_bool(NAME & S_IRUSR), \
    "mode.user.write", serialize_bool(NAME & S_IWUSR), \
    "mode.user.execute", serialize_bool(NAME & S_IXUSR), \
    "mode.group.read", serialize_bool(NAME & S_IRGRP), \
    "mode.group.write", serialize_bool(NAME & S_IWGRP), \
    "mode.group.execute", serialize_bool(NAME & S_IXGRP), \
    "mode.other.read", serialize_bool(NAME & S_IROTH), \
    "mode.other.write", serialize_bool(NAME & S_IWOTH), \
    "mode.other.execute", serialize_bool(NAME & S_IXOTH)

#define ACCESS_MODE_SERIALIZE(NAME) \
    "access.exists", serialize_bool(NAME == F_OK), \
    "access.readable", serialize_bool(NAME & R_OK), \
    "access.writable", serialize_bool(NAME & W_OK), \
    "access.executable", serialize_bool(NAME & X_OK)

#define VAR_ARGS_CONCAT_STR_SERIALIZE(FARG, VARGS, KEY) \
    KEY, serialize_vararg_to_string_null_terminated(FARG, VARGS)

#define ARGV_CONCAT_STR_SERIALIZE(NAME, KEY) \
    KEY, serialize_argv_to_string(NAME)

char * serialize_argv_to_string(char *const argv[]);
char * serialize_vararg_to_string_null_terminated(const char * farg, ...);
char * serialize_int(int i);
char * serialize_long(long l);
char * serialize_bool(int b);
char * serialize_timespec(struct timespec spec);
char * serialize_fd_filename(int fd);

#endif
