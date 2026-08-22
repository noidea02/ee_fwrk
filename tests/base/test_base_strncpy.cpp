#include <array>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr std::string_view REGULAR_STRV("This is a regular string.");
}

TEST(ee_fwrk_base, strncpy_empty_dst) {

    std::array<ee_char8_t, 0> dst{};
    EXPECT_EQ(ee_strncpy(dst.data(), std::string(REGULAR_STRV).c_str(), dst.size()), dst.data());
}

TEST(ee_fwrk_base, strncpy_empty_src) {

    std::array<ee_char8_t, 1> dst{};
    EXPECT_EQ(ee_strncpy(dst.data(), "", dst.size()), dst.data());
    EXPECT_EQ(dst[0], 0);
}

TEST(ee_fwrk_base, strncpy_empty_dst_and_src) {

    std::array<ee_char8_t, 0> dst{};
    EXPECT_EQ(ee_strncpy(dst.data(), "", dst.size()), dst.data());
}

TEST(ee_fwrk_base, strncpy_too_small_buf) {

    std::array<ee_char8_t, REGULAR_STRV.size()> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size()>(REGULAR_STRV, REGULAR_STRV.size() - 1) };

    ASSERT_GT(dst.size(), 0);
    EXPECT_EQ(ee_strncpy(dst.data(), std::string(REGULAR_STRV).c_str(), dst.size()), dst.data());
    EXPECT_EQ(dst[dst.size() - 1], 0);
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strncpy_barely_big_enough_buf) {

    std::array<ee_char8_t, REGULAR_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() + 1>(REGULAR_STRV) };

    EXPECT_EQ(ee_strncpy(dst.data(), std::string(REGULAR_STRV).c_str(), dst.size()), dst.data());
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strncpy_dst_bigger_than_src) {

    std::array<ee_char8_t, REGULAR_STRV.size() * 2> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() * 2>(REGULAR_STRV) };

    EXPECT_EQ(ee_strncpy(dst.data(), std::string(REGULAR_STRV).c_str(), dst.size()), dst.data());
    EXPECT_STREQ(dst.data(), expected.data());
}