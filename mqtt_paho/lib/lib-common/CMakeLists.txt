cmake_minimum_required(VERSION 3.13)

include(FindPackageHandleStandardArgs)

#------------------------------------------------------------------------------#
#    Main configuration                                                        #
#------------------------------------------------------------------------------#

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wpedantic)

if("$ENV{ARCH}" STREQUAL "arm")
    set(CMAKE_FIND_ROOT_PATH "$ENV{SDKTARGETSYSROOT}")
    find_package(sreproto REQUIRED)
    find_package(cslclapi REQUIRED)
endif()

#------------------------------------------------------------------------------#
#    Target run configuration                                                  #
#------------------------------------------------------------------------------#

find_program(QEMU_ARM_EXECUTABLE qemu-arm)
find_package_handle_standard_args(qemu-arm
    REQUIRED_VARS QEMU_ARM_EXECUTABLE
)

function(add_target_run TARGET)
    if(NOT QEMU_ARM_EXECUTABLE)
        return()
    endif()

    add_custom_target(${TARGET}-run
        COMMAND env QEMU_BIN="${QEMU_ARM_EXECUTABLE}"
                    QEMU_LD_PREFIX="$ENV{SDKTARGETSYSROOT}"
                    "${QEMU_ARM_EXECUTABLE}" "$<TARGET_FILE:${TARGET}>"
        DEPENDS ${TARGET}
    )
endfunction(add_target_run)

#------------------------------------------------------------------------------#
#    Cpplint configuration                                                     #
#------------------------------------------------------------------------------#

find_program(CPPLINT_EXECUTABLE cpplint.py)
find_package_handle_standard_args(cpplint
    REQUIRED_VARS CPPLINT_EXECUTABLE
)

set(CPPLINT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")

function(add_cpplint NAME)
    if(NOT CPPLINT_EXECUTABLE)
        return()
    endif()

    add_custom_target(${NAME}-cpplint
        COMMAND "${CPPLINT_EXECUTABLE}" --root="${CPPLINT_ROOT_DIR}" --linelength=120 ${ARGN}
    )
endfunction(add_cpplint)

#------------------------------------------------------------------------------#
#    Subprojects inclusion                                                     #
#------------------------------------------------------------------------------#

add_subdirectory(mocks EXCLUDE_FROM_ALL)
add_subdirectory(mqtt_util EXCLUDE_FROM_ALL)
add_subdirectory(test_util EXCLUDE_FROM_ALL)
add_subdirectory(util EXCLUDE_FROM_ALL)
