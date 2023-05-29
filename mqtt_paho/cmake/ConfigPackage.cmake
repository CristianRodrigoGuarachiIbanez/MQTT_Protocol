# #------------------------------------------------------------------------------#
# #    Example usage of config package                                           #
# #------------------------------------------------------------------------------#
#
# # Include the following snippet in application CMakeLists.txt
#
# include(ConfigPackage)
# set(CONFIG_PACKAGE_NAME "example-app-config")
# set(CONFIG_PACKAGE_DESCRIPTION "Example app config")
# set(CONFIG_PACKAGE_MAINTAINER "Example app Team <example_team@rio.cloud>")
# add_config_package(
#     ${CONFIG_PACKAGE_NAME}
#     ${FULL_VERSION}
#     ${CONFIG_PACKAGE_DESCRIPTION}
#     ${CONFIG_PACKAGE_MAINTAINER}
#     COPY "${PROJECT_APPCONF_FILE}:/etc/example_app"
# )
#
# #------------------------------------------------------------------------------#

add_custom_target(config_package)

function(parse_args prefix _singleArgNames _multiArgNames)
    foreach(arg_name ${_singleArgNames} ${_multiArgNames})
        set(${prefix}_${arg_name})
    endforeach()

    set(insideValues FALSE)
    set(currentArgName)
    foreach(currentArg ${ARGN})
        list(FIND _singleArgNames "${currentArg}" singleArgIndex)
        list(FIND _multiArgNames "${currentArg}" multiArgIndex)

        if(${singleArgIndex} EQUAL -1 AND ${multiArgIndex} EQUAL -1)
            if(insideValues)
                if("${insideValues}" STREQUAL "SINGLE")
                    set(${prefix}_${currentArgName} ${currentArg})
                    set(insideValues FALSE)
                elseif("${insideValues}" STREQUAL "MULTI")
                    list(APPEND ${prefix}_${currentArgName} ${currentArg})
                endif()
            endif()
        else()
            if(NOT ${singleArgIndex} EQUAL -1)
                set(currentArgName ${currentArg})
                set(insideValues "SINGLE")
            elseif(NOT ${multiArgIndex} EQUAL -1)
                set(currentArgName ${currentArg})
                set(insideValues "MULTI")
            endif()
        endif()
    endforeach()

    foreach(arg_name ${_singleArgNames} ${_multiArgNames})
        set(${prefix}_${arg_name}  ${${prefix}_${arg_name}} PARENT_SCOPE)
    endforeach()
endfunction(parse_args)

function(add_config_package ARG_NAME ARG_VERSION ARG_DESCRIPTION ARG_MAINTAINER)
    parse_args(ARG
        "args"
        "COPY;MKDIR"
        ${ARGN}
    )

    set(PACKAGE_OUTPUT_PATH    "${CMAKE_CURRENT_BINARY_DIR}/${ARG_NAME}_${ARG_VERSION}_${CPACK_OPKG_PACKAGE_ARCHITECTURE}")
    set(PACKAGE_OUTPUT_PACKAGE "${PACKAGE_OUTPUT_PATH}.ipk")
    set(PACKAGE_ROOT_PATH      "${PACKAGE_OUTPUT_PATH}/data")

    set(COPY_OUTPUTS)
    foreach(VAL IN LISTS ARG_COPY)
        string(REPLACE ":" ";" SPLITTED_LIST "${VAL}")
        list(GET SPLITTED_LIST 0 SRC_ABS_PATH)
        list(GET SPLITTED_LIST 1 DST_REL_DIR)

        if("${SRC_ABS_PATH}" STREQUAL "")
            message(FATAL_ERROR "Source path to file or directory is not specified")
        endif()

        get_filename_component(SRC_BASENAME "${SRC_ABS_PATH}" NAME)
        set(OUTPUT_PATH "${PACKAGE_ROOT_PATH}/${DST_REL_DIR}/${SRC_BASENAME}")
        add_custom_command(
            OUTPUT "${OUTPUT_PATH}"
            COMMAND "${CMAKE_COMMAND}" -E copy "${SRC_ABS_PATH}" "${OUTPUT_PATH}"
            DEPENDS "${SRC_ABS_PATH}"
            COMMENT "Coping ${SRC_ABS_PATH} to ${DST_REL_DIR}/${SRC_BASENAME}"
        )

        list(APPEND COPY_OUTPUTS "${OUTPUT_PATH}")
    endforeach()

    set(MKDIR_OUTPUTS)
    foreach(DST_REL_DIR IN LISTS ARG_MKDIR)
        if("${DST_REL_DIR}" STREQUAL "")
            message(FATAL_ERROR "Destination directory is not specified")
        endif()

        set(OUTPUT_PATH "${PACKAGE_ROOT_PATH}/${DST_REL_DIR}")
        add_custom_command(
            OUTPUT "${OUTPUT_PATH}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${OUTPUT_PATH}"
            COMMENT "Creating config directory ${DST_REL_DIR}"
        )

        list(APPEND MKDIR_OUTPUTS "${OUTPUT_PATH}")
    endforeach()

    set(PACKAGE_DEBIAN_BINARY_INFO          "debian-binary")
    set(PACKAGE_DEBIAN_BINARY_INFO_PATH     "${PACKAGE_OUTPUT_PATH}/${PACKAGE_DEBIAN_BINARY_INFO}")
    set(PACKAGE_CONTROL_INFO                "control")
    set(PACKAGE_CONTROL_INFO_PATH           "${PACKAGE_OUTPUT_PATH}/${PACKAGE_CONTROL_INFO}")
    set(PACKAGE_CONTROL_INFO_TGZ            "${PACKAGE_CONTROL_INFO_PATH}.tar.gz")
    set(PACKAGE_DATA                        "data")
    set(PACKAGE_DATA_PATH                   "${PACKAGE_OUTPUT_PATH}/${PACKAGE_DATA}")
    set(PACKAGE_DATA_TGZ                    "${PACKAGE_DATA_PATH}.tar.gz")

    file(WRITE ${PACKAGE_DEBIAN_BINARY_INFO_PATH} "2.0\n")
    file(WRITE ${PACKAGE_CONTROL_INFO_PATH}
        "Package: ${ARG_NAME}\n"
        "Version: ${ARG_VERSION}\n"
        "Description: ${ARG_DESCRIPTION}\n"
        "Architecture: ${CPACK_OPKG_PACKAGE_ARCHITECTURE}\n"
        "Section: extras\n"
        "Priority: optional\n"
        "Maintainer: ${ARG_MAINTAINER}\n"
        "Source: None\n"
        "License: CLOSED\n"
    )

    add_custom_command(
        OUTPUT "${PACKAGE_CONTROL_INFO_TGZ}"
        COMMAND "tar" "czf" "${PACKAGE_CONTROL_INFO_TGZ}" "-C" "${PACKAGE_OUTPUT_PATH}" "${PACKAGE_CONTROL_INFO}"
        DEPENDS ${PACKAGE_CONTROL_INFO_PATH}
    )

    add_custom_command(
        OUTPUT "${PACKAGE_DATA_TGZ}"
        COMMAND "tar" "czf" "${PACKAGE_DATA_TGZ}" "-C" "${PACKAGE_DATA_PATH}" "."
        DEPENDS ${COPY_OUTPUTS} ${MKDIR_OUTPUTS}
    )

    add_custom_command(
        OUTPUT "${PACKAGE_OUTPUT_PACKAGE}"
        COMMAND "ar" "r" "${PACKAGE_OUTPUT_PACKAGE}" "${PACKAGE_DEBIAN_BINARY_INFO_PATH}" "${PACKAGE_CONTROL_INFO_TGZ}" "${PACKAGE_DATA_TGZ}"
        DEPENDS ${COPY_OUTPUTS} ${MKDIR_OUTPUTS} ${PACKAGE_DEBIAN_BINARY_INFO_PATH} ${PACKAGE_CONTROL_INFO_TGZ} ${PACKAGE_DATA_TGZ}
    )

    add_custom_target(${ARG_NAME} DEPENDS "${PACKAGE_OUTPUT_PACKAGE}")
    add_dependencies(config_package ${ARG_NAME})

endfunction(add_config_package)
