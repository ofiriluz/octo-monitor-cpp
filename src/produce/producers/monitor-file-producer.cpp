/**
 * @file monitor-file-producer.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/produce/producers/monitor-file-producer.hpp"
#include <fstream>
#include <unistd.h>
#include <sys/stat.h>

namespace octo::monitor::produce
{
    MonitorFileProducer::MonitorFileProducer(const std::string & events_dir)
    : events_dir_(events_dir)
    {
        // Incase the events dir doesnt exist
        mkdir(events_dir.c_str(), 0700);
    }

    bool MonitorFileProducer::produce(const comm::MonitorEvent & event)
    {
        // Create event folder if it does not exist yet
        mkdir((events_dir_ + "/" + event.event_name()).c_str(), 0700);

        std::string serialized = event.serialize();
        std::string event_path = events_dir_ + "/" + event.event_name() + "/" + event.event_uuid() + ".json";
        std::ofstream out(event_path, std::ios::out);
        if(out.is_open())
        {
            out << serialized;
            out.close();
            return true;
        }
        return false;
    }
}
