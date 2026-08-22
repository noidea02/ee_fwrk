#include <array>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr auto EMPTY_CSTR{ "" };
    constexpr auto REGULAR_CSTR{ "This is a test string." };
}

TEST(ee_fwrk_base, deobfuscate_str_retval_guarantee) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    std::array<ee_byte_t, regular_str.size()> obf_out{};
    std::array<ee_char8_t, regular_str.size() + 1> deobf_out{};

    ee_obfuscate_str(std::string(regular_str).c_str(), obf_out.data(), obf_out.size());
    EXPECT_EQ(ee_deobfuscate_str(obf_out.data(), obf_out.size(), deobf_out.data(), deobf_out.size()), deobf_out.data());
}

TEST(ee_fwrk_base, deobfuscate_empty_str) {

    constexpr std::string_view empty_str(EMPTY_CSTR);
    static constexpr std::array<ee_char8_t, 1> expected{};
    std::array<ee_byte_t, empty_str.size()> obf_out{};
    std::array<ee_char8_t, 1> deobf_out{};

    ee_obfuscate_str(std::string(empty_str).c_str(), obf_out.data(), obf_out.size());
    EXPECT_EQ(ee_deobfuscate_str(obf_out.data(), obf_out.size(), deobf_out.data(), deobf_out.size()), deobf_out.data());
    EXPECT_EQ(deobf_out, expected);
}

TEST(ee_fwrk_base, deobfuscate_regular_str) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, regular_str.size() + 1>(regular_str) };
    std::array<ee_byte_t, regular_str.size()> obf_out{};
    std::array<ee_char8_t, regular_str.size() + 1> deobf_out{};

    ee_obfuscate_str(std::string(regular_str).c_str(), obf_out.data(), obf_out.size());
    EXPECT_EQ(ee_deobfuscate_str(obf_out.data(), obf_out.size(), deobf_out.data(), deobf_out.size()), deobf_out.data());
    EXPECT_EQ(deobf_out, expected);
}

TEST(ee_fwrk_base, deobfuscate_str_with_zero_len_buf) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    std::array<ee_byte_t, regular_str.size()> obf_out{};
    std::array<ee_char8_t, 0> deobf_out{};

    ASSERT_GT(obf_out.size(), 0);
    ASSERT_EQ(deobf_out.size(), 0);

    ee_obfuscate_str(std::string(regular_str).c_str(), obf_out.data(), obf_out.size());
    EXPECT_EQ(ee_deobfuscate_str(obf_out.data(), obf_out.size(), deobf_out.data(), deobf_out.size()), deobf_out.data());
}

TEST(ee_fwrk_base, deobfuscate_str_with_too_small_buf) {

    constexpr std::string_view regular_str(REGULAR_CSTR);
    constexpr auto num_deobf_chars{ regular_str.size() / 2 };
    static constexpr auto expected{ make_array_with_n_from_range<ee_char8_t, num_deobf_chars + 1>(regular_str, num_deobf_chars) };
    std::array<ee_byte_t, regular_str.size()> obf_out{};
    std::array<ee_char8_t, expected.size()> deobf_out{};

    ASSERT_LT(deobf_out.size(), regular_str.size() + 1);

    ee_obfuscate_str(std::string(regular_str).c_str(), obf_out.data(), obf_out.size());
    EXPECT_EQ(ee_deobfuscate_str(obf_out.data(), obf_out.size(), deobf_out.data(), deobf_out.size()), deobf_out.data());
    EXPECT_EQ(deobf_out, expected);
}