/**
 * @file monitor-event.hpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef MONITOR_EVENT_HPP_
#define MONITOR_EVENT_HPP_

#include <string>
#include <map>

namespace octo::monitor::comm
{
    class MonitorEvent
    {
        private:
            std::string event_uuid_;
            std::string event_name_;
            std::string event_protocol_version_;
            std::string event_type_;
            int event_timestamp_;
            std::map<std::string, std::string> event_data_;
            std::map<std::string, std::string> event_headers_;

        public:
            MonitorEvent();
            virtual ~MonitorEvent() = default;

            [[nodiscard]] std::string serialize() const;
            void deserialize(const std::string & json);

            [[nodiscard]] std::string event_uuid() const;
            [[nodiscard]] std::string event_protocol_version() const;
            [[nodiscard]] std::string event_name() const;
            [[nodiscard]] std::string event_type() const;
            MonitorEvent & set_event_type(const std::string & event_type);
            [[nodiscard]] int event_timestamp() const;
            MonitorEvent & set_event_name(const std::string & event_name);
            [[nodiscard]] std::map<std::string, std::string> event_data() const;
            MonitorEvent & set_event_data(const std::map<std::string, std::string> & event_data);
            MonitorEvent & add_event_data(const std::string & key, const std::string & val);
            MonitorEvent & extend_event_header(const std::string & key, const std::string & val);
    };
}

#endif
