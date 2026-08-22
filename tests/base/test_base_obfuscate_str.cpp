#include <array>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr auto EMPTY_CSTR{ "" };
    constexpr auto REGULAR_CSTR{ "This is a test string." };
}

TEST(ee_fwrk_base, obfuscate_str_retval_guarantee) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    std::array<ee_byte_t, regular_str.size()> out{};

    EXPECT_EQ(ee_obfuscate_str(std::string(regular_str).c_str(), out.data(), out.size()), out.data());
}

TEST(ee_fwrk_base, obfuscate_empty_str) {

    static constexpr auto expected{ make_array<ee_byte_t>(1, 2, 3) };
    auto out{ expected };

    ASSERT_EQ(expected, out);
    EXPECT_EQ(ee_obfuscate_str(EMPTY_CSTR, out.data(), out.size()), out.data());
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_base, obfuscate_regular_str) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    static constexpr auto not_expected{ make_array_with_n_from_range<ee_byte_t, regular_str.size()>(regular_str) };
    auto out{ not_expected };

    ASSERT_EQ(out, not_expected);
    EXPECT_EQ(ee_obfuscate_str(std::string(regular_str).c_str(), out.data(), out.size()), out.data());
    EXPECT_NE(out, not_expected);
}

TEST(ee_fwrk_base, obfuscate_str_zero_len_buf) {

    std::array<ee_byte_t, 0> out{};

    ASSERT_GT(std::string_view(REGULAR_CSTR).size(), 0);
    ASSERT_EQ(out.size(), 0);
    EXPECT_EQ(ee_obfuscate_str(REGULAR_CSTR, out.data(), out.size()), out.data());
}

TEST(ee_fwrk_base, obfuscate_str_too_small_buf) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    constexpr auto out_size{ regular_str.size() / 2 };
    static constexpr auto not_expected{ make_array_with_n_from_range<ee_byte_t, out_size>(regular_str) };
    auto out{ not_expected };

    ASSERT_LT(out.size(), regular_str.size());
    ASSERT_EQ(out, not_expected);
    EXPECT_EQ(ee_obfuscate_str(std::string(regular_str).c_str(), out.data(), out.size()), out.data());
    EXPECT_NE(out, not_expected);
}