# Check if CXX flags have been set to c++11 -> Setup Eclipse Indexer correctly!
# Also setup the project slightly different
if (${CMAKE_EXTRA_GENERATOR} MATCHES "Eclipse CDT4" )
    SET(CXX_ENABLED 0)
    LIST(LENGTH CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS LIST_LEN)
    if ( ${LIST_LEN} GREATER 0 )
        SET(CXX_ENABLED 1)
    endif()

    SET(C_ENABLED 0)
    LIST(LENGTH CMAKE_EXTRA_GENERATOR_C_SYSTEM_DEFINED_MACROS LIST_LEN)
    if ( ${LIST_LEN} GREATER 0)
        SET(C_ENABLED 1)
    endif()

    if (${C_ENABLED} EQUAL 1 AND ${CXX_ENABLED} EQUAL 1)
        # Combined project (C and CXX). This will confuse the indexer. For that reason
        # we unsert set the __cplusplus variable for the indexer, assuming C++ is only
        # used for unit tests!
        list(FIND CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS "__cplusplus" GEN_MACRO_INDEX)
        if( ${GEN_MACRO_INDEX} GREATER -1 )
            list(REMOVE_AT CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${GEN_MACRO_INDEX})
            list(REMOVE_AT CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${GEN_MACRO_INDEX})
        endif()
        SET(CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS} CACHE INTERNAL "")
    elseif ( (${CXX_ENABLED} EQUAL 1) AND (${CMAKE_CXX_FLAGS} MATCHES ".*-std=c\\+\\+11.*"))
        #add_definitions (-D__cplusplus=201103L)
        # CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS
        list(FIND CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS "199711L" GEN_MACRO_INDEX)
        if( ${GEN_MACRO_INDEX} GREATER -1 )
            list(REMOVE_AT CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${GEN_MACRO_INDEX})
            list(INSERT CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${GEN_MACRO_INDEX} "201103L")
            SET(CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS ${CMAKE_EXTRA_GENERATOR_CXX_SYSTEM_DEFINED_MACROS} CACHE INTERNAL "")
        endif()
    endif()
    # add additional includes which are necessary for the CDT indexer when cross compiling
    if (DEFINED ENV{SDKTARGETSYSROOT})
        include_directories(SYSTEM $ENV{SDKTARGETSYSROOT}/usr/include/c++/4.9.3
                                   $ENV{SDKTARGETSYSROOT}/usr/include/c++/4.9.3/arm-poky-linux-gnueabi
                                   $ENV{SDKTARGETSYSROOT}/usr/include/c++/4.9.3/backward
                                   $ENV{SDKTARGETSYSROOT}/../x86_64-pokysdk-linux/usr/lib/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/4.9.3/include
                                   $ENV{SDKTARGETSYSROOT}/../x86_64-pokysdk-linux/usr/lib/arm-poky-linux-gnueabi/gcc/arm-poky-linux-gnueabi/4.9.3/include-fixed
                            )
    endif()
endif()
