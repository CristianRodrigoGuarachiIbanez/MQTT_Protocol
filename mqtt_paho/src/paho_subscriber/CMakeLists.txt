cmake_minimum_required(VERSION 3.13.4)

project(MQTTSub)

add_library(${SUB_LIB} OBJECT MQTTSubscriber.cpp)

#add_executable(${PROJECT_NAME} mqtt_sub.cpp main.cpp)

#target_link_libraries(${PROJECT_NAME} paho-mqtt3cs)
