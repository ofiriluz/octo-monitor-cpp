/**
 * @file monitor-controller.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/monitor-controller.hpp"
#include "octo-monitor-cpp/comm/clients/monitor-uds-client.hpp"
#include "octo-monitor-cpp/produce/producers/monitor-comm-client-producer.hpp"
#include "octo-monitor-cpp/produce/producers/monitor-file-producer.hpp"
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>
#include <uuid.h>
#include <string.h>

namespace
{
    constexpr const auto DEFAULT_EVENTS_FILE_PATH = "/tmp/octo-events.json";
    constexpr const auto DEFAULT_UDS_PRODUCER_PATH = "/tmp/octo-uds-events";
    constexpr const auto DEFAULT_ENABLE_UDS_PRODUCER = false;
    constexpr const auto DEFAULT_ENABLE_FILESYSTEM_PRODUCER = true;

    constexpr const auto EVENTS_FILE_PATH_ENV_VAR = "EVENTS_FILE_PATH";
    constexpr const auto UDS_PRODUCER_PATH_ENV_VAR = "UDS_PRODUCER_PATH";
    constexpr const auto ENABLE_UDS_PRODUCER_ENV_VAR = "ENABLE_UDS_PRODUCER";
    constexpr const auto ENABLE_FILESYSTEM_PRODUCER_ENV_VAR = "ENABLE_FILESYSTEM_PRODUCER";
}

namespace octo::monitor
{
    MonitorController::MonitorController()
    : is_controller_running_(false)
    {

    }

    MonitorController & MonitorController::instance()
    {
        static MonitorController controller;
        return controller;
    }

    bool MonitorController::is_env_enabled(const std::string & env_name) const
    {
        auto env_is_enabled = std::getenv(env_name.c_str());
        return env_is_enabled && strlen(env_is_enabled) > 0;
    }

    void MonitorController::send_control_event(const std::string & name)
    {
        comm::MonitorEvent event;
        event.set_event_name(name);
        event.set_event_type("control");
        event.add_event_data("preloader_uuid", preloader_uuid_);
        extend_event(event);
        publish_event_to_producers(event);
    }

    void MonitorController::events_queue_thread()
    {
        // Send the initialization event
        send_control_event("preloader-initialization");

        while(is_controller_running_)
        {
            std::unique_lock<std::mutex> lock(events_queue_mtx_);
            events_queue_cv_.wait(lock, [&](){return events_queue_.size() > 0 || !is_controller_running_;});
            if(is_controller_running_) 
            {
                comm::MonitorEvent & next_event = events_queue_.front();
                publish_event_to_producers(next_event);
                events_queue_.pop_front();
            }
        }

        // Cleanup remaining events
        while(events_queue_.size() > 0)
        {
            comm::MonitorEvent & next_event = events_queue_.front();
            publish_event_to_producers(next_event);
            events_queue_.pop_front();
        }

        // Send the termination event
        send_control_event("preloader-termination");
    }

    std::string MonitorController::get_running_executable() const
    {
        char buffer[1024];
        int size = readlink("/proc/self/exe", buffer, 1024);
        buffer[size] = '\0';
        return std::string(buffer);
    }

    void MonitorController::publish_event_to_producers(const comm::MonitorEvent & event)
    {
        std::for_each(std::begin(producers_), 
                        std::end(producers_), 
                        [&](const produce::MonitorProducerUniquePtr & producer){ 
                            producer->produce(event); 
                        });
    }

    void MonitorController::extend_event(comm::MonitorEvent & event)
    {
        event.extend_event_header("executable", get_running_executable());
        event.extend_event_header("uid", std::to_string(getuid()));
        event.extend_event_header("gid", std::to_string(getgid()));
    }

    void MonitorController::produce_event(const comm::MonitorEvent & event)
    {
        std::unique_lock<std::mutex> lock(events_queue_mtx_);
        events_queue_.push_back(event);
        events_queue_cv_.notify_one();
    }

    void MonitorController::initialize_controller()
    {
        if(is_controller_running_)
        {
            return;
        }

        // Generate UUID for the preloader
        preloader_uuid_ = uuids::to_string(uuids::uuid_system_generator{}());
        is_controller_running_ = true;

        // Check if UDS client is enabled
        auto is_uds_enabled = DEFAULT_ENABLE_UDS_PRODUCER;
        if (is_env_enabled(ENABLE_UDS_PRODUCER_ENV_VAR))
        {
            is_uds_enabled = true;
        }
        if (is_uds_enabled)
        {
            auto uds_path = DEFAULT_UDS_PRODUCER_PATH;
            auto uds_env_path = std::getenv(UDS_PRODUCER_PATH_ENV_VAR);
            if (uds_env_path && strlen(uds_env_path) > 0)
            {
                uds_path = uds_env_path;
            }

            // Create the comm client to share between the consumers and producers
            comm_client_ = std::make_unique<comm::MonitorUDSClient>(uds_path);
            if(!comm_client_->open_comm())
            {
                throw std::runtime_error("Could not open UDS communication");
            }
            producers_.push_back(std::make_unique<produce::MonitorCommClientProducer>(comm_client_));
        }
        auto is_fs_enabled = DEFAULT_ENABLE_FILESYSTEM_PRODUCER;
        if (is_env_enabled(ENABLE_FILESYSTEM_PRODUCER_ENV_VAR))
        {
            is_fs_enabled = true;
        }        
        if (is_fs_enabled)
        {
            auto fs_path = DEFAULT_UDS_PRODUCER_PATH;
            auto fs_env_path = std::getenv(EVENTS_FILE_PATH_ENV_VAR);
            if (fs_env_path && strlen(fs_env_path) > 0)
            {
                fs_path = fs_env_path;
            }
            // File producer will create a loop on fclose, if you need a file producer, hide the fclose symbol
            producers_.push_back(std::make_unique<produce::MonitorFileProducer>(fs_path));
        }
        // Start the events queue thread
        events_queue_thread_.reset(new std::thread([&](){ events_queue_thread(); }));
    }

    void MonitorController::terminate_controller()
    {
        if(!is_controller_running_)
        {
            return;
        }

        // End the events queue
        is_controller_running_ = false;
        events_queue_thread_->join();
        events_queue_thread_.reset();

        // Close producers
        for(auto && prod : producers_)
        {
            prod.reset();
        }
    }

    std::string MonitorController::preloader_uuid() const
    {
        return preloader_uuid_;
    }

    bool MonitorController::is_thread_locked() const
    {
        return std::count(std::begin(locked_threads_), std::end(locked_threads_), std::this_thread::get_id());
    }

    void MonitorController::lock_thread()
    {
        std::unique_lock<std::mutex> lock(locked_threads_mtx_);
        locked_threads_.emplace(std::this_thread::get_id());
    }

    void MonitorController::unlock_thread()
    {
        std::unique_lock<std::mutex> lock(locked_threads_mtx_);
        locked_threads_.erase(std::this_thread::get_id());
    }
}
