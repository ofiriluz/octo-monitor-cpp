/**
 * @file monitor-comm-client-producer.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/produce/producers/monitor-comm-client-producer.hpp"

namespace octo::monitor::produce
{
    MonitorCommClientProducer::MonitorCommClientProducer(const comm::MonitorClientPtr & client)
    :client_(client)
    {

    }

    bool MonitorCommClientProducer::produce(const comm::MonitorEvent & event)
    {
        if(client_->is_comm_alive())
        {
            return client_->send_event(event);
        }
        return false;
    }
}
