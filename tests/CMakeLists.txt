include(FetchContent)
FetchContent_Declare(
    googletest
    URL      https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
    URL_HASH SHA256=40d4ec942217dcc84a9ebe2a68584ada7d4a33a8ee958755763278ea1c5e18ff
)
FetchContent_MakeAvailable(googletest)

set(base_test_sources
    base/test_base_utility_macros.cpp
    base/test_base_memcmp.cpp
    base/test_base_memcpy.cpp
    base/test_base_memmove.cpp
    base/test_base_memset.cpp
    base/test_base_strlen.cpp
    base/test_base_strncpy.cpp
    base/test_base_strcmp.cpp
    base/test_base_strapp.cpp
    base/test_base_strpad.cpp
    base/test_base_itoa32.cpp
    base/test_base_itoa64.cpp
    base/test_base_endian_platform_is_le.cpp
    base/test_base_endian_le_to_native_int16.cpp
    base/test_base_endian_le_to_native_int32.cpp
    base/test_base_endian_le_to_native_int64.cpp
    base/test_base_endian_be_to_native_int16.cpp
    base/test_base_endian_be_to_native_int32.cpp
    base/test_base_endian_be_to_native_int64.cpp
    base/test_base_endian_native_to_le_int16.cpp
    base/test_base_endian_native_to_le_int32.cpp
    base/test_base_endian_native_to_le_int64.cpp
    base/test_base_endian_native_to_be_int16.cpp
    base/test_base_endian_native_to_be_int32.cpp
    base/test_base_endian_native_to_be_int64.cpp
    base/test_base_obfuscation_macros.cpp
    base/test_base_obfuscate_str.cpp
    base/test_base_deobfuscate_str.cpp
    base/test_base_xor.cpp
    x86/test_x86_disasm_utils_sanity.cpp
    x86/test_x86_disasm_01h_add.cpp
    x86/test_x86_disasm_58h_vaddpd.cpp
    x86/test_x86_disasm_66h_0Fh_38h_F6h_adcx.cpp
    x86/test_x86_disasm_68h_push.cpp
    x86/test_x86_disasm_9Ah_call.cpp
    x86/test_x86_disasm_E9h_jmp.cpp
    x86/test_x86_disasm_FFh_inc.cpp
    x86/test_x86_disasm_chaining.cpp
    x86_format/test_x86_format.cpp
    x86_format/regress_x86_format.cpp
    pe/test_pe_read_image_info.cpp
    pe/test_pe_free_image_info.cpp
)

add_executable(ee_fwrk_test
    lib_impl.cpp
    ${base_test_sources}
)

target_link_libraries(ee_fwrk_test PRIVATE ee_fwrk ee_fwrk_support gtest_main)
set_target_properties(ee_fwrk_test PROPERTIES OUTPUT_NAME "test")
set_target_properties(ee_fwrk_test PROPERTIES CXX_STANDARD 23 CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS OFF)

target_compile_definitions(ee_fwrk_test PRIVATE EE_PE_TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/pe/test_data")

include(GoogleTest)
gtest_discover_tests(ee_fwrk_test)