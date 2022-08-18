/**
 * @file monitor-wrapper.c
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-02-10
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "octo-monitor-cpp/preloader/monitor-preloader.h"

void __attribute__ ((constructor)) init(void)
{
    initialize_preloader();
}

void __attribute__ ((destructor)) destroy(void)
{
    destroy_preloader();
}
