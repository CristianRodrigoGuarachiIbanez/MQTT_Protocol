

#Define Sources
file(GLOB SOURCES_MAIN "main.cpp")
file(GLOB SOURCES_PUB "paho_publisher/*.cpp")
file(GLOB SOURCES_SUB "paho_subscriber/*.cpp")

add_library(
        ${APP_NAME}_lib OBJECT
        ${SOURCES_PUB}
        ${SOURCES_SUB}
)

target_include_directories(${APP_NAME}_lib PUBLIC paho_publisher)
target_include_directories(${APP_NAME}_lib PUBLIC paho_subscriber)

target_link_libraries(
        ${APP_NAME}_lib
        ${LIBRARIES}
)

add_executable(
        ${APP_NAME}
        ${SOURCES_MAIN}
)

target_include_directories(
        ${APP_NAME}
        PRIVATE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_publisher>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/../../src/paho_subscriber>
)

target_link_libraries(
        ${APP_NAME}
        ${APP_NAME}_lib
)
