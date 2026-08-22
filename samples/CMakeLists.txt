function(ee_fwrk_add_sample target_name source_file_name)
    set(full_target_name ee_fwrk_sample_${target_name})
    set(combined_sources ${source_file_name} no_crt/no_crt_shared.c)

    if(WIN32)
        list(APPEND combined_sources no_crt/no_crt_win.c)
    elseif(LINUX)
        list(APPEND combined_sources no_crt/no_crt_linux.S no_crt/no_crt_linux.c)
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()

    add_executable(${full_target_name} ${combined_sources})
    target_link_libraries(${full_target_name} PRIVATE ee_fwrk)
    set_target_properties(${full_target_name} PROPERTIES OUTPUT_NAME "${target_name}")
    set_target_properties(${full_target_name} PROPERTIES C_STANDARD 90 C_STANDARD_REQUIRED ON C_EXTENSIONS OFF)

    if(MSVC)

        target_compile_options(${full_target_name} PRIVATE /GS- /Zc:threadSafeInit- /Oi-)
        target_link_options(${full_target_name} PRIVATE /NODEFAULTLIB /ENTRY:nc_main /SUBSYSTEM:CONSOLE)
        target_link_libraries(${full_target_name} PRIVATE kernel32 shell32)

    elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_C_COMPILER_ID MATCHES "Clang")

        target_compile_options(${full_target_name} PRIVATE -ffreestanding -fno-builtin -pedantic-errors -Wall -Wextra -Wdeclaration-after-statement -Wno-missing-field-initializers)
        target_link_options(${full_target_name} PRIVATE -nostdlib)

    endif()
endfunction()

if(MSVC)
    string(REPLACE "/RTC1" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
    string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()

ee_fwrk_add_sample(pe_info pe_info.c)
ee_fwrk_add_sample(x86_disasm x86_disasm.c)