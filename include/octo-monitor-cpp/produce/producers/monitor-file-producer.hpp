/**
 * @file monitor-file-producer.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_FILE_PRODUCER_HPP_
#define MONITOR_FILE_PRODUCER_HPP_

#include "octo-monitor-cpp/produce/monitor-producer.hpp"

namespace octo::monitor::produce
{
    class MonitorFileProducer : public MonitorProducer
    {
        private:
            std::string events_dir_;

        public:
            MonitorFileProducer(const std::string & events_dir);
            ~MonitorFileProducer() override = default;

            [[nodiscard]] bool produce(const comm::MonitorEvent & event) override;
    };
}

#endif
