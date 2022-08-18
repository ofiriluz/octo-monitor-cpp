octo-preloader-cpp
==============

[![Preloader Linux Build Pipeline](https://github.com/ofiriluz/octo-monitor-cpp/actions/workflows/linux.yml/badge.svg)](https://github.com/ofiriluz/octo-monitor-cpp/actions/workflows/linux.yml)

[![Preloader Mac Build Pipeline](https://github.com/ofiriluz/octo-monitor-cpp/actions/workflows/mac.yml/badge.svg)](https://github.com/ofiriluz/octo-monitor-cpp/actions/workflows/mac.yml)

Monitoring infrastructure for processes

The main idea is to load a preloader to a given process, such that the actions that the process performs, are outputted with the syscalls information, to either file or UDS

Each monitor event is outputted accordingly

Usage
-----

The preloader library can be used as follows while running a process:

```bash
LD_PRELOAD=libocto-monitor-cpp.so ./myprocess
```

Events coming out will be outputted in the following format
```json
{
    "event_header": {
        "event_uuid": "d53a84c2-a728-493a-89dc-db65c5ba6901",
        "event_protocol_version": "0.0.1",
        "event_timestamp": 1660830665,
        "executable": "/usr/bin/ls",
        "uid": 1050,
        "gid": 1050
    },
    "event_name": "open",
    "event_data": {
        "pathname": "/tmp",
        "open.read": true,
        "open.write": false,
        "open.append": false,
        "open.truncate": false,
        "open.create": false
    },
    "event_type": "syscall"
}
```

Each syscall is serialized in a similar manner
