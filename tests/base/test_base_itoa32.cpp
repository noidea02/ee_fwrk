#include <array>
#include <limits>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

TEST(ee_fwrk_base, itoa32_zero_len_buf) {

    std::array<ee_char8_t, 0> out{};
    EXPECT_FALSE(ee_itoa32(0x1122, out.data(), out.size(), EE_TRUE));
}

TEST(ee_fwrk_base, itoa32_too_small_buf) {

    std::array<ee_char8_t, 3> out{};
    EXPECT_FALSE(ee_itoa32(0x1122, out.data(), out.size(), EE_TRUE));
}

TEST(ee_fwrk_base, itoa32_barely_too_small_buf) {

    std::array<ee_char8_t, 4> out{};
    EXPECT_FALSE(ee_itoa32(0x1122, out.data(), out.size(), EE_TRUE));
}

TEST(ee_fwrk_base, itoa32_barely_big_enough_buf) {

    std::array<ee_char8_t, 5> out{};
    
    EXPECT_TRUE(ee_itoa32(0x1122, out.data(), out.size(), EE_TRUE));
    EXPECT_STREQ(out.data(), "1122");
}

TEST(ee_fwrk_base, itoa32_min) {

    constexpr auto in{ std::numeric_limits<ee_int32_t>::min() };
    std::array<ee_char8_t, 12> out{};
    
    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_FALSE));
    EXPECT_STREQ(out.data(), std::to_string(in).c_str());
}

TEST(ee_fwrk_base, itoa32_zero) {

    std::array<ee_char8_t, 2> out{};

    EXPECT_TRUE(ee_itoa32(0, out.data(), out.size(), EE_FALSE));
    EXPECT_STREQ(out.data(), "0");
}

TEST(ee_fwrk_base, itoa32_max) {

    constexpr auto in{ std::numeric_limits<ee_int32_t>::max() };
    std::array<ee_char8_t, 12> out{};

    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_FALSE));
    EXPECT_STREQ(out.data(), std::to_string(in).c_str());
}

TEST(ee_fwrk_base, itoa32_dec) {

    constexpr auto in{ 123 };
    std::array<ee_char8_t, 4> out{};

    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_FALSE));
    EXPECT_STREQ(out.data(), std::to_string(in).c_str());
}

TEST(ee_fwrk_base, itoa32_dec_negative) {

    constexpr auto in{ -123 };
    std::array<ee_char8_t, 5> out{};

    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_FALSE));
    EXPECT_STREQ(out.data(), std::to_string(in).c_str());
}

TEST(ee_fwrk_base, itoa32_hex) {

    constexpr auto in{ 123 };
    std::array<ee_char8_t, 3> out{};

    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_TRUE));
    EXPECT_STREQ(out.data(), "7B");
}

TEST(ee_fwrk_base, itoa32_hex_negative) {
    
    constexpr auto in{ -123 };
    std::array<ee_char8_t, 9> out{};

    EXPECT_TRUE(ee_itoa32(in, out.data(), out.size(), EE_TRUE));
    EXPECT_STREQ(out.data(), "FFFFFF85");
}