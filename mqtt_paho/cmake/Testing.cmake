
find_program(GCOV_EXECUTABLE $ENV{CROSS_COMPILE}gcov)
find_program(FIND_EXECUTABLE find)

add_custom_target(check)
if(GCOV_EXECUTABLE)
    execute_process(COMMAND ${GCOV_EXECUTABLE} "--version"
                    OUTPUT_VARIABLE GCOV_VERSION
    )
    string(REGEX MATCH "[0-9]+\\.[0-9]+" GCOV_VERSION "${GCOV_VERSION}")
endif()

if(FIND_EXECUTABLE)
    execute_process(COMMAND ${FIND_EXECUTABLE} "--version"
                    OUTPUT_VARIABLE FIND_VERSION
    )
    string(REGEX MATCH "[0-9]+\\.[0-9]+" FIND_VERSION "${FIND_VERSION}")
endif() 

function(filter_main_file VAR)
    set(${VAR})
    foreach(SOURCE ${ARGN})
        if(NOT SOURCE MATCHES ".*/main\\.c(c|pp)?")
            list(APPEND ${VAR} ${SOURCE})
        endif()
    endforeach()
    set(${VAR} ${${VAR}} PARENT_SCOPE)
endfunction(filter_main_file)

function(get_test_binary VAR)
    set(${VAR})
    foreach(TEST_TARGET ${ARGN})
        list(APPEND VAR "${TEST_TARGET}_bin")
    endforeach()
    set(${VAR} ${${VAR}} PARENT_SCOPE)
endfunction(get_test_binary)

function(add_unit_test ARG_NAME)

    cmake_parse_arguments(ARG "" ""
        "APP_SOURCES;TEST_SOURCES;DEFINITIONS;OPTIONS;LIBRARIES;ARGS"
        ${ARGN}
    )

    filter_main_file(APP_SOURCES_NOMAIN ${ARG_APP_SOURCES})
    get_test_binary(TEST_BINARY ${ARG_NAME})

    add_executable(${TEST_BINARY} EXCLUDE_FROM_ALL ${APP_SOURCES_NOMAIN} ${ARG_TEST_SOURCES})
    target_compile_definitions(${TEST_BINARY} PRIVATE ${ARG_DEFINITIONS})
    target_compile_options(${TEST_BINARY} PRIVATE -fprofile-arcs -ftest-coverage -fno-inline -fno-inline-small-functions ${ARG_OPTIONS})
    target_link_libraries(${TEST_BINARY} ${ARG_LIBRARIES}  gcov)
    #message(STATUS "${TARGET_FILE} ${TEST_BINARY}" )
    add_custom_target(${ARG_NAME}
        COMMAND "${CMAKE_COMMAND}" -E env TEST_APP="$<TARGET_FILE:${TEST_BINARY}>" 
        ${ARG_ARGS}
        DEPENDS ${TEST_BINARY}
    )
    add_dependencies(check ${ARG_NAME})
endfunction(add_unit_test)

function(add_module_test ARG_NAME)
    if(NOT QEMU_ARM_EXECUTABLE)
        return()
    endif()

    cmake_parse_arguments(ARG ""
        "RUN"
        "APP_SOURCES;TEST_SOURCES;DEFINITIONS;OPTIONS;LIBRARIES;DEPENDS;ARGS"
        ${ARGN}
    )

    filter_main_file(APP_SOURCES_NOMAIN ${ARG_APP_SOURCES})
    get_test_binary(TEST_BINARY ${ARG_NAME})

    add_executable(${TEST_BINARY} EXCLUDE_FROM_ALL ${APP_SOURCES_NOMAIN} ${ARG_TEST_SOURCES})
    target_compile_definitions(${TEST_BINARY} PRIVATE ${ARG_DEFINITIONS})
    target_compile_options(${TEST_BINARY} PRIVATE ${ARG_OPTIONS})
    target_link_libraries(${TEST_BINARY} ${ARG_LIBRARIES})

    add_custom_target(${ARG_NAME}
        COMMAND "${CMAKE_COMMAND}" -E env TEST_APP="$<TARGET_FILE:${TEST_BINARY}>"
                "${ARG_RUN}" ${ARG_ARGS}
        DEPENDS ${TEST_BINARY} ${ARG_DEPENDS}
    )

    add_dependencies(check ${ARG_NAME})
endfunction(add_module_test)

add_custom_target(clean-gcda
    COMMAND "${FIND_EXECUTABLE}" ${CMAKE_CURRENT_BINARY_DIR}
            -name *.gcda -type f -delete
    COMMENT "Cleaning gcov .gcda files"
)

add_custom_target(clean-gcno
    COMMAND "${FIND_EXECUTABLE}" ${CMAKE_CURRENT_BINARY_DIR}
            -name *.gcno -type f -delete
    COMMENT "Cleaning gcov .gcno files"
)

include(CoverageGcovr)
include(CoverageLcov)

function(add_coverage)
    if(COMMAND add_coverage_gcovr)
        add_coverage_gcovr(${ARGN})
    elseif(COMMAND add_coverage_lcov)
        add_coverage_lcov(${ARGN})
    endif()
endfunction(add_coverage)
