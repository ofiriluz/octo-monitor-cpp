/**
 * @file monitor-event.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/comm/monitor-event.hpp"
#include <nlohmann/json.hpp>
#include <uuid.h>

namespace
{
    constexpr const auto PROTOCOL_VERSION = "0.0.1";
}

namespace octo::monitor::comm
{
    MonitorEvent::MonitorEvent() : event_protocol_version_(PROTOCOL_VERSION)
    {
        // Set event timestamp
        event_timestamp_ = time(0);

        // Generate UUID for the event
        event_uuid_ = uuids::to_string(uuids::uuid_system_generator{}());
    }

    std::string MonitorEvent::serialize() const
    {
        nlohmann::json json;
        json["event_header"]["event_uuid"] = event_uuid_;
        json["event_header"]["event_protocol_version"] = event_protocol_version_;
        json["event_header"]["event_timestamp"] = event_timestamp_;
        for(auto && hdr : event_headers_) 
        {
            json["event_header"][hdr.first] = hdr.second;
        }
        json["event_name"] = event_name_;
        json["event_data"] = event_data_;
        json["event_type"] = event_type_;
        return json.dump(4);
    }

    void MonitorEvent::deserialize(const std::string & json)
    {
        nlohmann::json parsed = nlohmann::json::parse(json);
        event_uuid_ = parsed["event_header"]["event_uuid"];
        event_protocol_version_ = parsed["event_header"]["event_protocol_version"];
        event_timestamp_ = parsed["event_header"]["event_timestamp"];
        for(auto& element : parsed.at("event_header").items())
        {
            event_headers_[element.key()] = element.value();
        }
        event_name_ = parsed["event_name"];
        event_type_ = parsed["event_type"];
        for (auto& element : parsed.at("event_data").items()) 
        {
            event_data_[element.key()] = element.value();
        }
    }

    std::string MonitorEvent::event_uuid() const
    {
        return event_uuid_;
    }

    std::string MonitorEvent::event_protocol_version() const
    {
        return event_protocol_version_;
    }

    std::string MonitorEvent::event_name() const
    {
        return event_name_;
    }

    MonitorEvent & MonitorEvent::set_event_type(const std::string & event_type)
    {
        event_type_ = event_type;
        return *this;
    }

    int MonitorEvent::event_timestamp() const
    {
        return event_timestamp_;
    }

    MonitorEvent & MonitorEvent::set_event_name(const std::string & event_name)
    {
        event_name_ = event_name;
        return *this;
    }

    std::map<std::string, std::string> MonitorEvent::event_data() const
    {
        return event_data_;
    }

    MonitorEvent & MonitorEvent::set_event_data(const std::map<std::string, std::string> & event_data)
    {   
        event_data_ = event_data;
        return *this;
    }

    MonitorEvent & MonitorEvent::add_event_data(const std::string & key, const std::string & val)
    {
        event_data_[key] = val;
        return *this;
    }

    MonitorEvent & MonitorEvent::extend_event_header(const std::string & key, const std::string & val)
    {
        event_headers_[key] = val;
        return *this;
    }
}