/**
 * @file monitor-producer.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_PRODUCER_HPP_
#define MONITOR_PRODUCER_HPP_

#include "octo-monitor-cpp/comm/monitor-event.hpp"
#include <memory>

namespace octo::monitor::produce
{
    class MonitorProducer
    {
        public:
            MonitorProducer() = default;
            virtual ~MonitorProducer() = default;

            virtual bool produce(const comm::MonitorEvent & event) = 0;
    };
    typedef std::unique_ptr<MonitorProducer> MonitorProducerUniquePtr;
}

#endif
