/**
 * @file monitor-controller.h
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_CONTROLLER_HPP_
#define MONITOR_CONTROLLER_HPP_

#include <vector>
#include <set>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>
#include "octo-monitor-cpp/produce/monitor-producer.hpp"
#include "octo-monitor-cpp/comm/monitor-client.hpp"

namespace octo::monitor
{
    class MonitorController
    {
        private:
            std::vector<produce::MonitorProducerUniquePtr> producers_;
            std::set<std::thread::id> locked_threads_;
            std::deque<comm::MonitorEvent> events_queue_;
            comm::MonitorClientPtr comm_client_;
            std::string preloader_uuid_;
            std::unique_ptr<std::thread> events_queue_thread_;
            std::condition_variable events_queue_cv_;
            std::mutex events_queue_mtx_, locked_threads_mtx_;
            bool is_controller_running_;

        private:
            bool is_env_enabled(const std::string & env_name) const;
            void events_queue_thread();
            std::string get_running_executable() const;
            void send_control_event(const std::string & name);
            void publish_event_to_producers(const comm::MonitorEvent & event);
            

        protected:
            MonitorController();
            
        public:
            MonitorController(MonitorController const&) = delete;
            MonitorController(MonitorController&&) = delete;
            MonitorController & operator=(MonitorController const&) = delete;
            MonitorController & operator=(MonitorController &&) = delete;
            
            virtual ~MonitorController() = default;

        public:
            static MonitorController & instance();
            void extend_event(comm::MonitorEvent & event);
            void produce_event(const comm::MonitorEvent & event);
            void initialize_controller();
            void terminate_controller();
            [[nodiscard]] std::string preloader_uuid() const;
            [[nodiscard]] bool is_thread_locked() const;
            void lock_thread();
            void unlock_thread();
    };
}

#endif
