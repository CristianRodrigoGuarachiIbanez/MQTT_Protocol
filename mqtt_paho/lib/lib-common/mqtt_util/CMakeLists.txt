cmake_minimum_required(VERSION 3.1)

#------------------------------------------------------------------------------#
#    Main project configuration                                                #
#------------------------------------------------------------------------------#

project(platf-mqtt-util CXX)

#------------------------------------------------------------------------------#
#    Build library binary                                                      #
#------------------------------------------------------------------------------#

file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_link_libraries(${PROJECT_NAME} cslclapi platf-test-util platf-util)

target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_SOURCE_DIR}/include")

#------------------------------------------------------------------------------#
#    Add cpplint                                                               #
#------------------------------------------------------------------------------#

file(GLOB_RECURSE HEADERS "include/*.h")

add_cpplint(${PROJECT_NAME} ${SOURCES} ${HEADERS})
