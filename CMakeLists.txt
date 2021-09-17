CMAKE_MINIMUM_REQUIRED(VERSION 3.10.2)
PROJECT(lib-snowflakeid C)

# Settings
SET(CMAKE_C_STANDARD 99)
IF (MSVC)
    SET(CMAKE_C_FLAGS "-Wall")
ELSE (MSVC)
    SET(CMAKE_C_FLAGS "-Wall -Werror -pedantic -fPIC")
ENDIF (MSVC)
SET(CMAKE_CXX_FLAGS_DEBUG "-g")
SET(CMAKE_CXX_FLAGS_RELEASE "-O2")

# Configure version.h file
SET(LIBSNOWFLAKEID_VERSION_STRING "1.0.0")
SET(LIBSNOWFLAKEID_VERSION_NUMBER "100")
CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/res/version.h.template ${CMAKE_SOURCE_DIR}/src/version.h)

# Display build type
IF (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "Build type: -")
ELSE (NOT CMAKE_BUILD_TYPE)
    MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
ENDIF (NOT CMAKE_BUILD_TYPE)

# Detect "mutex" to use (WINDOWS or PTHREAD)
FIND_PACKAGE(Threads REQUIRED)
IF (CMAKE_USE_PTHREADS_INIT)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_PTHREAD_MUTEX=1")
ELSEIF (CMAKE_USE_WIN32_THREADS_INIT)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DUSE_WINDOWS_MUTEX=1")
ENDIF (CMAKE_USE_PTHREADS_INIT)

# Source Files
SET(SOURCE_FILES ${SOURCE_FILES}
        src/snowflakeid_destroy.c
        src/snowflakeid_get_version_as_int.c
        src/snowflakeid_get_version_as_str.c
        src/snowflakeid_initialize.c
        src/snowflakeid_next_value.c)

# Test Files
SET(TEST_FILES ${TEST_FILES}
        test/test_main.c)

# Library
ADD_LIBRARY(lib-snowflakeid-obj OBJECT ${SOURCE_FILES})
ADD_LIBRARY(lib-snowflakeid-static STATIC $<TARGET_OBJECTS:lib-snowflakeid-obj>)
ADD_LIBRARY(lib-snowflakeid-shared SHARED $<TARGET_OBJECTS:lib-snowflakeid-obj>)
IF (NOT MSVC)
    SET_TARGET_PROPERTIES(lib-snowflakeid-static PROPERTIES OUTPUT_NAME "snowflakeid")
ENDIF (NOT MSVC)
SET_TARGET_PROPERTIES(lib-snowflakeid-shared PROPERTIES OUTPUT_NAME "snowflakeid")

# Binary (Unit Tests)
ADD_EXECUTABLE(lib-snowflakeid-test $<TARGET_OBJECTS:lib-snowflakeid-obj> ${TEST_FILES})

# Install
IF (NOT MSVC)
    INSTALL(TARGETS lib-snowflakeid-static ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
ENDIF (NOT MSVC)
INSTALL(TARGETS lib-snowflakeid-shared ARCHIVE DESTINATION lib LIBRARY DESTINATION lib RUNTIME DESTINATION bin)
INSTALL(DIRECTORY ${CMAKE_SOURCE_DIR}/src/ DESTINATION include FILES_MATCHING PATTERN "*.h*")
