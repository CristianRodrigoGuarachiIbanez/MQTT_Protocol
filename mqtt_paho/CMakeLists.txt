cmake_minimum_required( VERSION 3.13)


# ------------------------------------------------------------------------------#
# Enable TBM3 cmake module directory                                            #
# ------------------------------------------------------------------------------#
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

project(MQTT)


set(APP_NAME ${PROJECT_NAME})
set(PUB_LIB "${PROJECT_NAME}-mqtt-pub")
set(SUB_LIB "${PROJECT_NAME}-mqtt-sub")
set(DEFAULT_CONF_DIR "/opt/man/current/etc/")
set(DEFAULT_CONF_FILE "${DEFAULT_CONF_DIR}${APP_NAME}.conf")


file(GLOB_RECURSE SOURCES "src/*.cpp" )
file(GLOB_RECURSE HEADERS "src/*.h")
list(APPEND SOURCE ${SOURCES} "src/main.cpp")

add_subdirectory(lib/googletest EXCLUDE_FROM_ALL)
#add_subdirectory(src/paho_publisher)
#add_subdirectory(src/paho_subscriber)

file(GLOB HEADER_PATHS 
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/paho_publisher"
    "${PROJECT_SOURCE_DIR}/src/paho_subscriber"
) 
include_directories(${HEADER_PATHS})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
    message(STATUS "dir='${dir}'")
endforeach()

set(LIBRARIES mosquitto paho-mqtt3cs)

add_subdirectory(src)

# ------------------------------------------------------------------------------#
# Add testing                                                                   #
# ------------------------------------------------------------------------------#
add_subdirectory(test)

# ------------------------------------------------------------------------------#
# Add local command                                                         #
# ------------------------------------------------------------------------------#
# add_custom_target(
#     unittest
#     DEPENDS ${PROJECT_NAME}
# )
# add_custom_command(
#     TARGET unittest
#     COMMAND "${CMAKE_COMMAND}" -L"$<TARGET_FILE:${APP_NAME}-test>"
#     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#     COMMENT "Running binary locally ..."
#     USES_TERMINAL
# )


