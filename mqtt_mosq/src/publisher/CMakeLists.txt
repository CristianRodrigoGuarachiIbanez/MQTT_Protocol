cmake_minimum_required(VERSION 3.13.4)

project(PUBLISHER)

add_library(${PROJECT_NAME} mqtt_pub.cpp)

#add_executable(${PROJECT_NAME} mqtt_pub.cpp main.cpp)

#target_link_libraries(${PROJECT_NAME} mosquitto)