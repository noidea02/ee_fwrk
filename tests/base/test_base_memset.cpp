#include <stdint.h>

#include <algorithm>
#include <array>
#include <ranges>
#include <span>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

TEST(ee_fwrk_base, memset_zero_length_set) {

    static constexpr auto expected{ make_array_fill<uint8_t, 3>(1) };
    auto dst{ expected };

    ASSERT_EQ(dst, expected);
    EXPECT_EQ(ee_memset(dst.data(), 2, 0), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memset_retval_guarantee) {

    static constexpr auto expected{ make_array_fill<uint8_t, 3>(1) };
    std::array<uint8_t, expected.size()> dst{};

    EXPECT_EQ(ee_memset(dst.data(), 1, dst.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memset_standard_charset) {

    static constexpr auto initial{ make_array<uint8_t>(0, 1, 2, 3, 4) };
    auto dst{ initial };
    static constexpr std::array<uint8_t, initial.size()> expected{};

    ASSERT_EQ(initial, dst);
    ASSERT_NE(dst, expected);
    ASSERT_EQ(dst.size(), 5);
    EXPECT_EQ(ee_memset(dst.data(), 0, dst.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memset_value_truncation) {

    std::array<uint8_t, 3> dst{};
    static constexpr auto expected{ make_array_fill<uint8_t, 3>(0x34) };

    ASSERT_NE(dst, expected);
    ASSERT_EQ(dst.size(), 3);
    EXPECT_EQ(ee_memset(dst.data(), (ee_byte_t)0x1234, dst.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memset_negative_value_casting) {

    std::array<uint8_t, 3> dst{};
    static constexpr auto expected{ make_array_fill<uint8_t, 3>(0xff) };

    ASSERT_NE(dst, expected);
    ASSERT_EQ(dst.size(), 3);
    EXPECT_EQ(ee_memset(dst.data(), (ee_byte_t)-1, dst.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memset_strict_boundary_adherence) {

    static constexpr auto initial{ make_array<uint8_t>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) };
    auto dst{ initial };
    static constexpr auto expected0_4{ make_array_fill<uint8_t, 5>(1) };
    static constexpr auto expected5_9{ make_array<uint8_t>(5, 6, 7, 8, 9) };

    ASSERT_EQ(initial, dst);
    ASSERT_EQ(dst.size(), 10);
    EXPECT_EQ(ee_memset(dst.data(), 1, 5), dst.data());

    std::array<uint8_t, dst.size() / 2> actual0_4{};
    std::ranges::copy(std::span(dst).first(5), actual0_4.begin());

    std::array<uint8_t, dst.size() / 2> actual5_9{};
    std::ranges::copy(std::span(dst).last(5), actual5_9.begin());

    EXPECT_EQ(actual0_4, expected0_4);
    EXPECT_EQ(actual5_9, expected5_9);
}

TEST(ee_fwrk_base, memset_single_byte_set) {

    static constexpr auto initial{ make_array<uint8_t>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) };
    auto dst{ initial };
    static constexpr auto expected0_4{ make_array<uint8_t>(1, 1, 2, 3, 4) };
    static constexpr auto expected5_9{ make_array<uint8_t>(5, 6, 7, 8, 9) };

    ASSERT_EQ(initial, dst);
    ASSERT_EQ(dst.size(), 10);
    EXPECT_EQ(ee_memset(dst.data(), 1, 1), dst.data());

    std::array<uint8_t, dst.size() / 2> actual0_4{};
    std::ranges::copy(std::span(dst).first(5), actual0_4.begin());

    std::array<uint8_t, dst.size() / 2> actual5_9{};
    std::ranges::copy(std::span(dst).last(5), actual5_9.begin());

    EXPECT_EQ(actual0_4, expected0_4);
    EXPECT_EQ(actual5_9, expected5_9);
}