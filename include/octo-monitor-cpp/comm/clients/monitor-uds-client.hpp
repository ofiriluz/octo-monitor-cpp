/**
 * @file monitor-uds-client.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_UDS_CLIENT_HPP_
#define MONITOR_UDS_CLIENT_HPP_

#include "octo-monitor-cpp/comm/monitor-client.hpp"
#include <octo-logger-cpp/logger.hpp>
#include <thread>

namespace
{
    constexpr int DEFAULT_UDS_POLL_TIMEOUT = 500;
}

namespace octo::monitor::comm
{
    class MonitorUDSClient : public MonitorClient
    {
        private:
            logger::Logger uds_comm_logger_;
            std::string uds_path_;
            std::unique_ptr<std::thread> uds_listener_thread_;
            int uds_fd_;
            bool is_opened_;
            int uds_poll_timeout_;

        private:
            void listen_thread();

        public:
            MonitorUDSClient(const std::string & path, int poll_timeout = DEFAULT_UDS_POLL_TIMEOUT);
            virtual ~MonitorUDSClient();

            [[nodiscard]] bool open_comm() override;
            [[nodiscard]] bool close_comm() override;
            [[nodiscard]] bool is_comm_alive() override;
            [[nodiscard]] bool send_event(const MonitorEvent & event) override;
    };
}

#endif
