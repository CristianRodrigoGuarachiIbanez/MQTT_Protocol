cmake_minimum_required(VERSION 3.13.4)

project(MQTT)


#add_library(
    #"${CMAKE_CURRENT_SOURCE_DIR}/subscriber/mqtt_pub.cpp"
    #"${CMAKE_CURRENT_SOURCE_DIR}/publisher/mqtt_sub.cpp"
#)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(publisher)
add_subdirectory(subscriber)

target_include_directories(${PROJECT_NAME} PUBLIC publisher)
target_include_directories(${PROJECT_NAME} PUBLIC subscriber)

target_link_directories(${PROJECT_NAME} PRIVATE publisher)
target_link_directories(${PROJECT_NAME} PRIVATE subscriber)

#target_compile_definitions(publisher subscriber PUBLIC )

target_link_libraries(${PROJECT_NAME} mosquitto PUBLISHER SUBSCRIBER )