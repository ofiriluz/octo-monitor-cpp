/**
 * @file monitor-client.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/comm/monitor-client.hpp"
#include <algorithm>

namespace octo::monitor::comm
{
    void MonitorClient::process_event(const MonitorEvent & event)
    {
        std::for_each(std::begin(callbacks_),
                        std::end(callbacks_),
                        [&](const MonitorClientCallback & callback){
                            callback(event);
                        });
    }

    void MonitorClient::add_event_listener(const MonitorClientCallback & callback)
    {
        callbacks_.push_back(callback);
    }
}
