if(NOT DEFINED PROJECT_NAME)
    message(FATAL_ERROR "PROJECT_NAME variable is not defined")
endif()

if(NOT DEFINED PROJECT_VERSION)
    message(FATAL_ERROR "PROJECT_VERSION variable is not defined")
endif()

if(DEFINED ENV{SRE_SDK_VERSION})
    set(FULL_VERSION "${PROJECT_VERSION}-V$ENV{SRE_SDK_VERSION}")
else()
    set(FULL_VERSION "${PROJECT_VERSION}")
endif()

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/versioninfo.txt" "${PROJECT_NAME} ${PROJECT_VERSION}")

if("$ENV{ARCH}" STREQUAL "arm")
    set(CMAKE_FIND_ROOT_PATH "$ENV{SDKTARGETSYSROOT}")
    find_package(sreproto REQUIRED)
    find_package(cslclapi REQUIRED)
endif()

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

include_directories("${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic -Wno-unused-parameter)
