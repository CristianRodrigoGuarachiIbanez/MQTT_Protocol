file(GLOB SOURCES_UNITTEST_SUB "unittest/unittest_pub/*.cpp")
file(GLOB SOURCES_UNITTEST_PUB "unittest/unittest_sub/*.cpp")
file(GLOB SOURCES_MAIN "unittest/*.cpp" )

message(STATUS "r -> ${LIBRARIES}")

enable_testing()
find_package(GTest REQUIRED)

add_executable(
        ${APP_NAME}-test
        ${SOURCES_UNITTEST_SUB}
        ${SOURCES_UNITTEST_PUB}
        ${SOURCES_MAIN}
)

target_include_directories(
        ${APP_NAME}-test
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_publisher>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_subscriber>
)

target_link_libraries(
        ${APP_NAME}-test
        ${APP_NAME}_lib
        ${LIBRARIES}
        gtest
        gtest_main
        gmock
)

add_executable(
        ${APP_NAME}-test-intelliJ
        EXCLUDE_FROM_ALL
        ${SOURCES_UNITTEST_SUB}
        ${SOURCES_UNITTEST_PUB}
        ${SOURCES_MAIN}
)

target_include_directories(
        ${APP_NAME}-test-intelliJ
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_publisher>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_subscriber>
)

target_link_libraries(
        ${APP_NAME}-test-intelliJ
        ${APP_NAME}_lib
        gtest
        gtest_main
        gmock
)


add_custom_command(TARGET ${APP_NAME}-test-intelliJ
        POST_BUILD
        COMMAND mv ${APP_NAME}-test-intelliJ ${APP_NAME}-test-intelliJ-real
        COMMAND echo "" >> ${APP_NAME}-test-intelliJ
        COMMAND chmod +x ${APP_NAME}-test-intelliJ
        VERBATIM
    )
