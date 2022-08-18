/**
 * @file monitor-comm-client-producer.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_COMM_CLIENT_PRODUCER_HPP_
#define MONITOR_COMM_CLIENT_PRODUCER_HPP_

#include "octo-monitor-cpp/produce/monitor-producer.hpp"
#include "octo-monitor-cpp/comm/monitor-client.hpp"

namespace octo::monitor::produce
{
    class MonitorCommClientProducer : public MonitorProducer
    {
        private:
            comm::MonitorClientPtr client_;

        public:
            MonitorCommClientProducer(const comm::MonitorClientPtr & client);
            ~MonitorCommClientProducer() override = default;

            [[nodiscard]] bool produce(const comm::MonitorEvent & event) override;
    };
}

#endif
