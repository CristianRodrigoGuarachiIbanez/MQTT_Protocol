#cmake_minimum_required(VERSION 3.13.4)

project(SUBSCRIBER)

add_library(${PROJECT_NAME} mqtt_sub.cpp)

#add_executable(${PROJECT_NAME} mqtt_sub.cpp main.cpp)

#target_link_libraries(${PROJECT_NAME} mosquitto)