CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

PROJECT(octo-preloader-cpp)

FIND_PACKAGE(octo-logger-cpp REQUIRED CONFIG)
FIND_PACKAGE(nlohmann_json REQUIRED CONFIG)
FIND_PACKAGE(stduuid REQUIRED CONFIG)

# Library definition
ADD_LIBRARY(octo-preloader-cpp SHARED
    src/monitor-controller.cpp
    src/comm/monitor-client.cpp
    src/comm/monitor-event.cpp
    src/comm/clients/monitor-uds-client.cpp
    src/produce/producers/monitor-comm-client-producer.cpp
    src/produce/producers/monitor-file-producer.cpp
    src/preloader/monitor-preloader.cpp
    src/preloader/wrappers/monitor-serializer.c
    src/preloader/wrappers/monitor-wrapper-access.c
    src/preloader/wrappers/monitor-wrapper-exec.c
    src/preloader/wrappers/monitor-wrapper-io.c
    src/preloader/wrappers/monitor-wrapper-open.c
    src/preloader/wrappers/monitor-wrapper-socket.c
    src/preloader/wrappers/monitor-wrapper-stat.c
    src/preloader/wrappers/monitor-wrapper.c
)

# Alias
ADD_LIBRARY(core::octo-preloader-cpp ALIAS octo-preloader-cpp)

# Properties
SET_TARGET_PROPERTIES(octo-preloader-cpp PROPERTIES CXX_STANDARD 17 POSITION_INDEPENDENT_CODE ON)

TARGET_COMPILE_OPTIONS(octo-preloader-cpp
    PRIVATE
        $<$<NOT:$<PLATFORM_ID:Windows>>:-Werror=return-type>
        $<$<NOT:$<PLATFORM_ID:Windows>>:-Werror=switch>
)

TARGET_COMPILE_DEFINITIONS(octo-preloader-cpp PUBLIC UUID_SYSTEM_GENERATOR=ON)

TARGET_INCLUDE_DIRECTORIES(octo-preloader-cpp
    PUBLIC
        # Logger includes
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

IF(NOT APPLE)
    TARGET_LINK_OPTIONS(
        octo-preloader-cpp 
        PRIVATE
            "-Wl,--version-script=${CMAKE_SOURCE_DIR}/config/symbols"
    )
ELSE()
    SET(${PROJECT_NAME}_FRAMEWORKS
        "-framework CoreFoundation"
        "-framework ApplicationServices"
        "-framework IOKit"
        "-framework AppKit"
        "-framework Security"
    )
ENDIF()

TARGET_LINK_LIBRARIES(octo-preloader-cpp
    octo::octo-logger-cpp
    stduuid::stduuid
    nlohmann_json::nlohmann_json
    ${${PROJECT_NAME}_FRAMEWORKS}
)

# Installation of the logger
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include
)

INSTALL(TARGETS octo-preloader-cpp
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
