#include <array>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#define EMPTY_SL ""
#define OBFUSCATED_EMPTY_SL ""
#define REGULAR_SL "This is a test string."
#define OBFUSCATED_REGULAR_SL "\x98\xa4\x3c\x27\xc8\x6d\x59\x61\x61\xab\x74\x39\x2e\xb7\xec\xbf\xb8\xbe\x3c\x3a\x8f\x2a"

namespace {

    constexpr auto REGULAR_SL_SIZE{ sizeof(REGULAR_SL) };
}

TEST(ee_fwrk_base, obfuscated_sl_empty) {

    std::array<ee_char8_t, 1> deobf{};
    EXPECT_EQ(EE_OBFUSCATED_SL(OBFUSCATED_EMPTY_SL, deobf.data(), deobf.size()), deobf.data());
    EXPECT_STREQ(deobf.data(), EMPTY_SL);
}

TEST(ee_fwrk_base, obfuscated_sl_regular) {

    std::array<ee_char8_t, REGULAR_SL_SIZE> deobf{};
    EXPECT_EQ(EE_OBFUSCATED_SL(OBFUSCATED_REGULAR_SL, deobf.data(), deobf.size()), deobf.data());
    EXPECT_STREQ(deobf.data(), REGULAR_SL);
}