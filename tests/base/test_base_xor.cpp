#include <array>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr auto REGULAR_INPUT{ make_array_incremental_fill<ee_byte_t, 16>(8) };
    constexpr auto REGULAR_KEY{ make_array_incremental_fill<ee_byte_t, 8>() };
}

TEST(ee_fwrk_base, xor_empty_in) {

    std::array<ee_byte_t, 0> in{};
    std::array<ee_byte_t, 1> out{};

    ASSERT_GT(out.size(), 0);
    ASSERT_EQ(out[0], 0);

    ee_xor(in.data(), in.size(), REGULAR_KEY.data(), REGULAR_KEY.size(), out.data(), out.size());
    EXPECT_EQ(out[0], 0);
}

TEST(ee_fwrk_base, xor_empty_key) {

    std::array<ee_byte_t, 0> key{};
    std::array<ee_byte_t, REGULAR_INPUT.size()> out{};
    static constexpr auto expected{ REGULAR_INPUT };

    ee_xor(REGULAR_INPUT.data(), REGULAR_INPUT.size(), key.data(), key.size(), out.data(), out.size());
    EXPECT_EQ(out, expected);
}

TEST(ee_fwrk_base, xor_empty_out) {

    std::array<ee_byte_t, 0> out{};
    ee_xor(REGULAR_INPUT.data(), REGULAR_INPUT.size(), REGULAR_KEY.data(), REGULAR_KEY.size(), out.data(), out.size());
}

TEST(ee_fwrk_base, xor_valid_in_and_key_and_out) {

    std::array<ee_byte_t, REGULAR_INPUT.size()> out{};

    ee_xor(REGULAR_INPUT.data(), REGULAR_INPUT.size(), REGULAR_KEY.data(), REGULAR_KEY.size(), out.data(), out.size());
    EXPECT_NE(out, REGULAR_INPUT);

    ee_xor(out.data(), out.size(), REGULAR_KEY.data(), REGULAR_KEY.size(), out.data(), out.size());
    EXPECT_EQ(out, REGULAR_INPUT);
}

TEST(ee_fwrk_base, xor_identical_in_and_key) {

    std::array<ee_byte_t, REGULAR_KEY.size()> out{};
    static constexpr std::array<ee_byte_t, REGULAR_KEY.size()> expected{};

    ee_xor(REGULAR_KEY.data(), REGULAR_KEY.size(), REGULAR_KEY.data(), REGULAR_KEY.size(), out.data(), out.size());
    EXPECT_EQ(out, expected);
}