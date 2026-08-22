cmake_minimum_required(VERSION 3.21)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

project(ee_fwrk VERSION 1.0.0 LANGUAGES ASM C CXX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} -g")
endif()

add_library(ee_fwrk INTERFACE)

target_include_directories(ee_fwrk INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

option(EE_FWRK_BUILD_TESTS "Build tests" ON)
option(EE_FWRK_BUILD_SAMPLES "Build samples" ON)
option(EE_FWRK_BUILD_TOOLS "Build tools" ON)

if(EE_FWRK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(EE_FWRK_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()

if(EE_FWRK_BUILD_TOOLS)
    add_subdirectory(tools)
endif()