#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

#define FIRST_PART_CSTR "This is the first part, "
#define SECOND_PART_CSTR "this is the second part."
#define COMBINED_CSTR FIRST_PART_CSTR SECOND_PART_CSTR

namespace {

    constexpr std::string_view FIRST_PART_STRV(FIRST_PART_CSTR);
    constexpr std::string_view SECOND_PART_STRV(SECOND_PART_CSTR);
    constexpr std::string_view COMBINED_STRV(COMBINED_CSTR);
}

static_assert(COMBINED_STRV.size() == (FIRST_PART_STRV.size() + SECOND_PART_STRV.size()));

TEST(ee_fwrk_base, strapp_zero_len_buf) {

    std::array<ee_char8_t, 0> dst;
    EXPECT_FALSE(ee_strapp(dst.data(), dst.size(), FIRST_PART_CSTR));
}

TEST(ee_fwrk_base, strapp_too_small_buf) {

    auto dst{ make_array_with_n_from_range<ee_char8_t, FIRST_PART_STRV.size() + 1>(FIRST_PART_STRV) };
    EXPECT_FALSE(ee_strapp(dst.data(), dst.size(), SECOND_PART_CSTR));
}

TEST(ee_fwrk_base, strapp_barely_too_small_buf) {

    auto dst{ make_array_with_n_from_range<ee_char8_t, COMBINED_STRV.size()>(FIRST_PART_STRV) };
    EXPECT_FALSE(ee_strapp(dst.data(), dst.size(), SECOND_PART_CSTR));
}

TEST(ee_fwrk_base, strapp_barely_big_enough_buf) {

    auto dst{ make_array_with_n_from_range<ee_char8_t, COMBINED_STRV.size() + 1>(FIRST_PART_STRV) };
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, COMBINED_STRV.size() + 1>(COMBINED_STRV) };

    EXPECT_TRUE(ee_strapp(dst.data(), dst.size(), SECOND_PART_CSTR));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strapp_empty_dst) {

    std::array<ee_char8_t, SECOND_PART_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, SECOND_PART_STRV.size() + 1>(SECOND_PART_STRV) };

    EXPECT_TRUE(ee_strapp(dst.data(), dst.size(), SECOND_PART_CSTR));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strapp_empty_src) {

    auto dst{ make_array_with_n_from_range<ee_char8_t, FIRST_PART_STRV.size() + 1>(FIRST_PART_STRV) };
    const auto expected{ dst };

    EXPECT_TRUE(ee_strapp(dst.data(), dst.size(), ""));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strapp_empty_dst_and_src) {

    std::array<ee_char8_t, 1> dst{};

    EXPECT_TRUE(ee_strapp(dst.data(), dst.size(), ""));
    EXPECT_STREQ(dst.data(), "");
}

TEST(ee_fwrk_base, strapp_regular_dst_and_src) {

    auto dst{ make_array_with_n_from_range<ee_char8_t, COMBINED_STRV.size() * 2>(FIRST_PART_STRV) };
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, COMBINED_STRV.size() + 1>(COMBINED_STRV) };

    EXPECT_TRUE(ee_strapp(dst.data(), dst.size(), SECOND_PART_CSTR));
    EXPECT_STREQ(dst.data(), expected.data());
}