cmake_minimum_required(VERSION 3.0)

#------------------------------------------------------------------------------#
#    Main project configuration                                                #
#------------------------------------------------------------------------------#

project(platf-mocks CXX)

#------------------------------------------------------------------------------#
#    Define library                                                            #
#------------------------------------------------------------------------------#

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/include")

#------------------------------------------------------------------------------#
#    Add cpplint                                                               #
#------------------------------------------------------------------------------#

file(GLOB_RECURSE HEADERS "include/*.h")

add_cpplint(${PROJECT_NAME} ${HEADERS})
