CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
PROJECT(lib-snowflakeid C)

# Settings
SET(CMAKE_C_STANDARD 99)
SET(CMAKE_C_FLAGS "-Wall -Werror -pedantic -fPIC")
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Display build type
IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "Build type: -")
ELSE (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
ENDIF (NOT CMAKE_BUILD_TYPE)

# Source Files
SET(SOURCE_FILES ${SOURCE_FILES}
        src/snowflakeid_destroy.c
        src/snowflakeid_initialize.c
        src/snowflakeid_next_value.c)

# Test Files
SET(TEST_FILES ${TEST_FILES}
        test/test_main.c)

# Library
ADD_LIBRARY(lib-snowflakeid-obj OBJECT ${SOURCE_FILES})
ADD_LIBRARY(lib-snowflakeid-static STATIC $<TARGET_OBJECTS:lib-snowflakeid-obj>)
ADD_LIBRARY(lib-snowflakeid-shared SHARED $<TARGET_OBJECTS:lib-snowflakeid-obj>)
SET_TARGET_PROPERTIES(lib-snowflakeid-static PROPERTIES OUTPUT_NAME "snowflakeid")
SET_TARGET_PROPERTIES(lib-snowflakeid-shared PROPERTIES OUTPUT_NAME "snowflakeid")

# Binary (Unit Tests)
ADD_EXECUTABLE(lib-snowflakeid-test $<TARGET_OBJECTS:lib-snowflakeid-obj> ${TEST_FILES})

# Install
INSTALL(TARGETS lib-snowflakeid-static ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
INSTALL(TARGETS lib-snowflakeid-shared ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.h*")
