/**
 * @file monitor-preloader.h
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_PRELOADER_HPP_
#define MONITOR_PRELOADER_HPP_

#define RC_OK 0
#define RC_THREAD_LOCKED -1

#ifdef __cplusplus
extern "C" 
{
#endif
    void initialize_preloader();
    void destroy_preloader();
    int on_event(const char * event_name, const char ** event_data);
#ifdef __cplusplus
}
#endif
#endif
