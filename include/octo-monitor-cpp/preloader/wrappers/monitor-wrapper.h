/**
 * @file monitor-wrapper.h
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_WRAPPER_HPP_
#define MONITOR_WRAPPER_HPP_

#define _GNU_SOURCE

#include <dlfcn.h>

#define ORIGINAL_FUNCTION(NAME) \
    dlsym(RTLD_NEXT, NAME)

#endif
