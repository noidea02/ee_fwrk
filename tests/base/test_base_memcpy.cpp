#include <stdint.h>

#include <algorithm>
#include <array>
#include <span>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

TEST(ee_fwrk_base, memcpy_zero_length) {

    auto dst{ std::array<uint8_t, 1>() };
    EXPECT_EQ(ee_memcpy(dst.data(), "hello", 0), dst.data());
    EXPECT_EQ(dst[0], 0);
}

TEST(ee_fwrk_base, memcpy_retval_guarantee) {

    static constexpr auto src{ make_array<uint8_t>(0, 1, 2) };
    std::array<uint8_t, src.size()> dst{};

    ASSERT_EQ(src.size(), dst.size());
    ASSERT_NE(src, dst);
    EXPECT_EQ(ee_memcpy(dst.data(), src.data(), dst.size()), dst.data());
    EXPECT_EQ(src, dst);
}

TEST(ee_fwrk_base, memcpy_standard_copy) {

    static constexpr auto src{ make_array<uint8_t>(0, 1, 2) };
    auto dst{ make_array<uint8_t>(0, 0, 0, 3) };
    static constexpr auto expected{ make_array<uint8_t>(0, 1, 2, 3) };

    ASSERT_LT(src.size(), dst.size());
    ASSERT_NE(dst, expected);
    EXPECT_EQ(ee_memcpy(dst.data(), src.data(), src.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memcpy_binary_null_byte_handling) {

    static constexpr auto src{ make_array<uint8_t>(1, 0, 2, 0, 3) };
    std::array<uint8_t, src.size()> dst{};

    ASSERT_EQ(sizeof(src), sizeof(dst));
    ASSERT_NE(src, dst);
    EXPECT_EQ(ee_memcpy(dst.data(), src.data(), dst.size()), dst.data());
    EXPECT_EQ(src, dst);
}

TEST(ee_fwrk_base, memcpy_adjacent_src_to_dst) {

    auto src_dst{ make_array<uint8_t>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) };
    ASSERT_EQ(src_dst.size(), 10);

    const auto src_range{ std::span(src_dst).subspan(0, 5) };
    const auto dst_range{ std::span(src_dst).subspan(5) };
    ASSERT_EQ(src_range.size_bytes(), dst_range.size_bytes());
    ASSERT_FALSE(std::ranges::equal(src_range, dst_range));

    EXPECT_EQ(ee_memcpy(dst_range.data(), src_range.data(), dst_range.size_bytes()), dst_range.data());
    EXPECT_TRUE(std::ranges::equal(src_range, dst_range));
}

TEST(ee_fwrk_base, memcpy_adjacent_dst_to_src) {

    auto src_dst{ make_array<uint8_t>(0, 1, 2, 3, 4, 5, 6, 7, 8, 9) };
    ASSERT_EQ(src_dst.size(), 10);

    const auto src_range{ std::span(src_dst).subspan(5) };
    const auto dst_range{ std::span(src_dst).subspan(0, 5) };
    ASSERT_EQ(src_range.size_bytes(), dst_range.size_bytes());
    ASSERT_FALSE(std::ranges::equal(src_range, dst_range));

    EXPECT_EQ(ee_memcpy(dst_range.data(), src_range.data(), dst_range.size_bytes()), dst_range.data());
    EXPECT_TRUE(std::ranges::equal(src_range, dst_range));
}