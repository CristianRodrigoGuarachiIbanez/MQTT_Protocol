cmake_minimum_required(VERSION 3.13.4)
project(MQTTPub)
add_library(${PUB_LIB} OBJECT MQTTPublisher.cpp)


# add_executable(${PROJECT_NAME} MQTTPublisher.cpp)

# target_link_libraries(${PROJECT_NAME} paho-mqtt3cs)
