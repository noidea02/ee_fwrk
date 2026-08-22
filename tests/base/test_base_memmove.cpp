#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

TEST(ee_fwrk_base, memmove_zero_length) {

    auto dst{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto src{ make_array<ee_uint8_t>(5, 6, 7, 8) };
    const auto expected{ dst };

    EXPECT_EQ(ee_memmove(dst.data(), src.data(), 0), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memmove_retval_guarantee) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    EXPECT_EQ(ee_memmove(buf.data(), buf.data(), buf.size()), buf.data());
}

TEST(ee_fwrk_base, memmove_matching_dst_src) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto expected{ buf };

    EXPECT_EQ(ee_memmove(buf.data(), buf.data(), buf.size()), buf.data());
    EXPECT_EQ(buf, expected);
}

TEST(ee_fwrk_base, memmove_distinct_dst_src) {

    auto dst{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto src{ make_array<ee_uint8_t>(5, 6, 7, 8) };
    const auto expected{ src };

    ASSERT_EQ(dst.size(), src.size());
    EXPECT_EQ(ee_memmove(dst.data(), src.data(), dst.size()), dst.data());
    EXPECT_EQ(dst, expected);
}

TEST(ee_fwrk_base, memmove_overlapping_dst_offset_1) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto expected{ make_array<ee_uint8_t>(1, 1, 2, 3) };

    EXPECT_EQ(ee_memmove(buf.data() + 1, buf.data(), buf.size() - 1), buf.data() + 1);
    EXPECT_EQ(buf, expected);
}

TEST(ee_fwrk_base, memmove_overlapping_dst_offset_2) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto expected{ make_array<ee_uint8_t>(1, 2, 1, 2) };

    EXPECT_EQ(ee_memmove(buf.data() + 2, buf.data(), buf.size() - 2), buf.data() + 2);
    EXPECT_EQ(buf, expected);
}

TEST(ee_fwrk_base, memmove_overlapping_src_offset_1) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto expected{ make_array<ee_uint8_t>(2, 3, 4, 4) };

    EXPECT_EQ(ee_memmove(buf.data(), buf.data() + 1, buf.size() - 1), buf.data());
    EXPECT_EQ(buf, expected);
}

TEST(ee_fwrk_base, memmove_overlapping_src_offset_2) {

    auto buf{ make_array<ee_uint8_t>(1, 2, 3, 4) };
    const auto expected{ make_array<ee_uint8_t>(3, 4, 3, 4) };

    EXPECT_EQ(ee_memmove(buf.data(), buf.data() + 2, buf.size() - 2), buf.data());
    EXPECT_EQ(buf, expected);
}