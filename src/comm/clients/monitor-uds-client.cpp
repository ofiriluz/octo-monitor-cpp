/**
 * @file monitor-uds-client.cpp
 * @author ofir iluz (iluzofir@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-08-11
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "octo-monitor-cpp/comm/clients/monitor-uds-client.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>

namespace octo::monitor::comm
{
    void MonitorUDSClient::listen_thread()
    {
        // Listen and parse json events
        int rv;
        struct pollfd fds[1];
        fds[0].fd = uds_fd_;
        fds[0].events = POLLIN | POLLPRI;
        int buffer_size;
        char * buffer;
        while(is_opened_)
        {
            // Poll for data
            if((rv = poll(fds, 1, uds_poll_timeout_)) > 0)
            {
                if(fds[0].revents & POLLIN)
                {
                    // Receive the event buffer size
                    rv = recv(uds_fd_, (char*)&buffer_size, sizeof(buffer_size), 0);
                    if(rv > 0 && buffer_size > 0)
                    {
                        // Receive the event
                        buffer = new char[buffer_size];
                        rv = recv(uds_fd_, buffer, buffer_size, 0);
                        if(rv > 0)
                        {
                            // Create the monitor event
                            MonitorEvent event;
                            event.deserialize(std::string(buffer));

                            // Publish the event
                            process_event(event);
                        }
                    }
                }
            }
        }
    }

    MonitorUDSClient::MonitorUDSClient(const std::string & path, int poll_timeout)
    : uds_comm_logger_("MonitorUDSClient"), uds_path_(path), uds_fd_(-1), uds_poll_timeout_(poll_timeout)
    {

    }

    MonitorUDSClient::~MonitorUDSClient()
    {
        if (!close_comm())
        {
            uds_comm_logger_.warning() << "Failed to close UDS comm";
        }
    }

    bool MonitorUDSClient::open_comm()
    {
        struct sockaddr_un addr;
        // Open the UDS socket and connect it to the agent
        if((uds_fd_ = socket(PF_UNIX, SOCK_STREAM, 0)) < 0)
        {
            uds_comm_logger_.error() << "Could not open UDS socket [(" << errno << ", " << strerror(errno) << ")]";
            return false;
        }

        // Bindand connect to the socket
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strcpy(addr.sun_path, uds_path_.c_str());

        if(connect(uds_fd_, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        {
            uds_comm_logger_.error() << "Could not connect to the UDS socket [(" << errno << ", " << strerror(errno) << ")]";
            close(uds_fd_);
            return false;
        }

        // Start the listener thread
        is_opened_ = true;
        uds_listener_thread_ = std::unique_ptr<std::thread>(
                                    new std::thread([&](){listen_thread();}));
        return true;
    }

    bool MonitorUDSClient::close_comm()
    {
        is_opened_ = false;
        if(uds_fd_ > 0)
        {
            close(uds_fd_);
            uds_fd_ = -1;
        }
        if(uds_listener_thread_)
        {
            uds_listener_thread_->join();
            uds_listener_thread_.reset();
        }

        return true;
    }

    bool MonitorUDSClient::is_comm_alive()
    {
        return is_opened_ && uds_fd_ > 0;
    }

    bool MonitorUDSClient::send_event(const MonitorEvent & event)
    {
        // Make sure the socket is connected
        if(!is_comm_alive())
        {
            return false;
        }

        // Serialize the event
        std::string serialized_event = event.serialize();

        // Send event size and then send the event itself
        int size = serialized_event.size();
        if(send(uds_fd_, (char*)&size, sizeof(int), 0) > 0)
        {
            return send(uds_fd_, serialized_event.c_str(), serialized_event.size(), 0) > 0;
        }
        return false;
    }
}
