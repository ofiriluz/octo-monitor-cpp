/**
 * @file monitor-client.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_CLIENT_HPP_
#define MONITOR_CLIENT_HPP_

#include "octo-monitor-cpp/comm/monitor-event.hpp"
#include <functional>
#include <memory>
#include <vector>

namespace octo::monitor::comm
{
    typedef std::function<void(const MonitorEvent &)> MonitorClientCallback;
    class MonitorClient
    {
        private:
            std::vector<MonitorClientCallback> callbacks_;

        protected:
            void process_event(const MonitorEvent & event);

        public:
            MonitorClient() = default;
            virtual ~MonitorClient() = default;

            void add_event_listener(const MonitorClientCallback & callback);

        public:
            virtual bool open_comm() = 0;
            virtual bool close_comm() = 0;
            virtual bool is_comm_alive() = 0;
            virtual bool send_event(const MonitorEvent & event) = 0;
    };
    typedef std::shared_ptr<MonitorClient> MonitorClientPtr;
}

#endif
