#include <array>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

#define REGULAR_SL "This is a regular string."

namespace {

    constexpr std::string_view REGULAR_STRV(REGULAR_SL);
    constexpr std::string_view BACK_PADDED_STRV(REGULAR_SL "aa");
    constexpr std::string_view FRONT_PADDED_STRV("aa" REGULAR_SL);
}

TEST(ee_fwrk_base, strpad_copy_too_small_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size()> dst{};
    EXPECT_FALSE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size(), EE_FALSE, 0, dst.data(), dst.size()));
}

TEST(ee_fwrk_base, strpad_copy_barely_big_enough_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() + 1>(REGULAR_STRV) };

    EXPECT_TRUE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size(), EE_FALSE, 0, dst.data(), dst.size()));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strpad_pad_back_too_small_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size() + 2> dst{};
    EXPECT_FALSE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size() + 2, EE_FALSE, 'a', dst.data(), dst.size()));
}

TEST(ee_fwrk_base, strpad_pad_back_barely_big_enough_dst) {

    std::array<ee_char8_t, BACK_PADDED_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, BACK_PADDED_STRV.size() + 1>(BACK_PADDED_STRV, BACK_PADDED_STRV.size()) };

    EXPECT_TRUE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size() + 2, EE_FALSE, 'a', dst.data(), dst.size()));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strpad_pad_front_too_small_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size() + 2> dst{};
    EXPECT_FALSE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size() + 2, EE_TRUE, 'a', dst.data(), dst.size()));
}

TEST(ee_fwrk_base, strpad_pad_front_barely_big_enough_dst) {

    std::array<ee_char8_t, FRONT_PADDED_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, FRONT_PADDED_STRV.size() + 1>(FRONT_PADDED_STRV, FRONT_PADDED_STRV.size()) };

    EXPECT_TRUE(ee_strpad(std::string(REGULAR_STRV).c_str(), REGULAR_STRV.size() + 2, EE_TRUE, 'a', dst.data(), dst.size()));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strpad_crop_back_too_small_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size()> dst{};
    EXPECT_FALSE(ee_strpad(std::string(BACK_PADDED_STRV).c_str(), REGULAR_STRV.size(), EE_FALSE, 0, dst.data(), dst.size()));
}

TEST(ee_fwrk_base, strpad_crop_back_barely_big_enough_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() + 1>(REGULAR_STRV) };

    EXPECT_TRUE(ee_strpad(std::string(BACK_PADDED_STRV).c_str(), REGULAR_STRV.size(), EE_FALSE, 0, dst.data(), dst.size()));
    EXPECT_STREQ(dst.data(), expected.data());
}

TEST(ee_fwrk_base, strpad_crop_front_too_small_dst) {

    std::array<ee_char8_t, REGULAR_STRV.size()> dst{};
    EXPECT_FALSE(ee_strpad(std::string(FRONT_PADDED_STRV).c_str(), REGULAR_STRV.size(), EE_TRUE, 0, dst.data(), dst.size()));
}

TEST(ee_fwrk_base, strpad_crop_front_barely_big_enough_dst) {
    
    std::array<ee_char8_t, REGULAR_STRV.size() + 1> dst{};
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, REGULAR_STRV.size() + 1>(REGULAR_STRV) };

    EXPECT_TRUE(ee_strpad(std::string(FRONT_PADDED_STRV).c_str(), REGULAR_STRV.size(), EE_TRUE, 0, dst.data(), dst.size()));
    EXPECT_STREQ(dst.data(), expected.data());
}