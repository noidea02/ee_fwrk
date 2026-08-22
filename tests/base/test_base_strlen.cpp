#include <array>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr std::string_view REGULAR_STRV("This is a regular string.");
    
    constexpr std::string_view NON_PRINTABLE_STRV("\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\x11\x12"
        "\x13\x14\x15\x16\x17\x18\x19\x1A\x1B\x1C\x1D\x1E\x1F\x20");
}

TEST(ee_fwrk_base, strlen_empty_str) {
    EXPECT_EQ(ee_strlen(""), 0);
}

TEST(ee_fwrk_base, strlen_regular_str) {
    EXPECT_EQ(ee_strlen(std::string(REGULAR_STRV).c_str()), REGULAR_STRV.size());
}

TEST(ee_fwrk_base, strlen_non_printable_str) {
    EXPECT_EQ(ee_strlen(std::string(NON_PRINTABLE_STRV).c_str()), NON_PRINTABLE_STRV.size());
}

TEST(ee_fwrk_base, strlen_two_terminators) {

    static constexpr auto str0{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() + 1>(REGULAR_STRV) };
    static constexpr auto str1{ str0 };
    static constexpr auto combined_str{ concat_arrays(str0, str1) };

    EXPECT_EQ(ee_strlen(combined_str.data()), REGULAR_STRV.size());
}