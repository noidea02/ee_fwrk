add_library(ee_fwrk_support INTERFACE)

target_include_directories(ee_fwrk_support INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)