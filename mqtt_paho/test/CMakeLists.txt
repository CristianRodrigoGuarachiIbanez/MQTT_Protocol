file(GLOB SOURCES_UNITTEST_PUB "unittest_pub/*.cpp")
file(GLOB SOURCES_UNITTEST_SUB "unittest_sub/*.cpp")
file(GLOB FILE_MANAGER  "fileManager/*cpp")
file(GLOB SOURCES_MAIN "./*.cpp" )

message(STATUS "r -> ${LIBRARIES}")

enable_testing()
find_package(GTest REQUIRED)

add_executable(
        ${APP_NAME}-test-sub
        ${SOURCES_UNITTEST_SUB}
        ${FILE_MANAGER}
)

target_include_directories(
        ${APP_NAME}-test-sub
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_subscriber>
)

target_link_libraries(
        ${APP_NAME}-test-sub
        ${APP_NAME}_lib
        ${LIBRARIES}
        gtest
        gtest_main
        gmock
)

add_executable(
        ${APP_NAME}-test-pub
        ${SOURCES_UNITTEST_PUB}
        ${FILE_MANAGER}
)

target_include_directories(
        ${APP_NAME}-test-pub
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_publisher>
)

target_link_libraries(
        ${APP_NAME}-test-pub
        ${APP_NAME}_lib
        ${LIBRARIES}
        gtest
        gtest_main
        gmock
)


add_executable(
        ${APP_NAME}-unittest
        ${FILE_MANAGER}
        ${SOURCES_MAIN}
)

target_include_directories(
        ${APP_NAME}-unittest
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_subscriber>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_publisher>
)

target_link_libraries(
        ${APP_NAME}-unittest
        ${APP_NAME}_lib
        ${LIBRARIES}
        gtest
        gtest_main
        gmock
)

# add_custom_command(TARGET ${APP_NAME}-test-intelliJ
#         POST_BUILD
#         COMMAND mv ${APP_NAME}-test-intelliJ ${APP_NAME}-test-intelliJ-real
#         COMMAND echo "" >> ${APP_NAME}-test-intelliJ
#         COMMAND chmod +x ${APP_NAME}-test-intelliJ
#         VERBATIM
#     )
