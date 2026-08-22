#include <stdint.h>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

TEST(ee_fwrk_base, memcmp_zero_length) {
    
    EXPECT_EQ(ee_memcmp(nullptr, nullptr, 0), 0);
    EXPECT_EQ(ee_memcmp("abc", "def", 0), 0);
}

TEST(ee_fwrk_base, memcmp_identical_pointers) {

    static constexpr auto mem{ make_array<uint8_t>(0, 1, 2) };
    EXPECT_EQ(ee_memcmp(mem.data(), mem.data(), 0), 0);
    EXPECT_EQ(ee_memcmp(mem.data(), mem.data(), mem.size()), 0);
}

TEST(ee_fwrk_base, memcmp_exact_equality) {

    static constexpr auto mem0{ make_array<uint8_t>(0, 1, 2) };
    static constexpr auto mem1{ make_array<uint8_t>(0, 1, 2) };

    ASSERT_EQ(mem0, mem1);
    EXPECT_EQ(ee_memcmp(mem0.data(), mem1.data(), mem0.size()), 0);
}

TEST(ee_fwrk_base, memcmp_difference_at_first_byte) {

    static constexpr auto mem0{ make_array<uint8_t>(0, 1, 2) };
    static constexpr auto mem1{ make_array<uint8_t>(1, 1, 2) };

    ASSERT_EQ(mem0.size(), mem1.size());
    EXPECT_LT(ee_memcmp(mem0.data(), mem1.data(), mem0.size()), 0);
    EXPECT_GT(ee_memcmp(mem1.data(), mem0.data(), mem0.size()), 0);
}

TEST(ee_fwrk_base, memcmp_difference_at_last_byte) {

    static constexpr auto mem0{ make_array<uint8_t>(0, 1, 2) };
    static constexpr auto mem1{ make_array<uint8_t>(0, 1, 3) };

    ASSERT_EQ(mem0.size(), mem1.size());
    EXPECT_LT(ee_memcmp(mem0.data(), mem1.data(), mem0.size()), 0);
    EXPECT_GT(ee_memcmp(mem1.data(), mem0.data(), mem0.size()), 0);
}

TEST(ee_fwrk_base, memcmp_difference_beyond_n) {

    static constexpr auto mem0{ make_array<uint8_t>(0, 1, 2) };
    static constexpr auto mem1{ make_array<uint8_t>(0, 1, 3) };

    ASSERT_GT(mem0.size(), 0);
    ASSERT_EQ(mem0.size(), mem1.size());
    EXPECT_EQ(ee_memcmp(mem0.data(), mem1.data(), mem0.size() - 1), 0);
}

TEST(ee_fwrk_base, memcmp_ensure_uchar_interpretation) {

    constexpr uint8_t chr0{ 0x00 };
    constexpr uint8_t chr1{ 0xff };

    ASSERT_EQ(sizeof(chr0), sizeof(chr1));
    EXPECT_LT(ee_memcmp(&chr0, &chr1, sizeof(chr0)), 0);
}

TEST(ee_fwrk_base, memcmp_high_bit_mismatch) {

    constexpr uint8_t chr0{ 0x7f };
    constexpr uint8_t chr1{ 0x80 };

    ASSERT_EQ(sizeof(chr0), sizeof(chr1));
    EXPECT_LT(ee_memcmp(&chr0, &chr1, sizeof(chr0)), 0);
}