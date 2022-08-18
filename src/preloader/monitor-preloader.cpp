/**
 * @file monitor-preloader.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/preloader/monitor-preloader.h"
#include "octo-monitor-cpp/monitor-controller.hpp"
#include <octo-logger-cpp/manager.hpp>
#include <sstream>
#include <string>
#include <memory>

extern "C"
{
    void initialize_preloader()
    {
        // Init logger
        std::shared_ptr<octo::logger::ManagerConfig> config(new octo::logger::ManagerConfig());
        octo::logger::SinkConfig console_sink_config("Console", octo::logger::SinkConfig::SinkType::CONSOLE_SINK);
        config->add_sink(console_sink_config);
        octo::logger::Manager::instance().configure(config);

        // Init controller
        octo::monitor::MonitorController::instance().initialize_controller();
    }

    void destroy_preloader()
    {
        // Teardown controller
        octo::monitor::MonitorController::instance().terminate_controller();
    }

    int on_event(const char * event_name, const char ** event_data)
    {
        // Check if thread id is locked
        if(octo::monitor::MonitorController::instance().is_thread_locked())
        {
            return RC_THREAD_LOCKED;
        }
        // Lock the thread id
        octo::monitor::MonitorController::instance().lock_thread();

        // Create the event
        octo::monitor::comm::MonitorEvent event;

        // Add the event info
        event.set_event_name(event_name);
        event.set_event_type("syscall");
        std::map<std::string, std::string> event_data_map;
        event_data_map["preloader_uuid"] = octo::monitor::MonitorController::instance().preloader_uuid();
        while(*event_data) 
        {
            std::string key = *event_data;
            event_data++;
            if(*event_data)
            {
                std::string value = *event_data;
                event_data_map[key] = value;
            }
            else
            {
                break;
            }
            event_data++;
        }
        event.set_event_data(event_data_map);

        // Extend the event headers and produce it
        octo::monitor::MonitorController::instance().extend_event(event);
        octo::monitor::MonitorController::instance().produce_event(event);
        
        // Unlock the thread
        octo::monitor::MonitorController::instance().unlock_thread();
        return RC_OK;
    }
}
