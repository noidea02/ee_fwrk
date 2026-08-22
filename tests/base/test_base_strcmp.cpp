#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr std::string_view FIRST_STRV("First string.");
    constexpr std::string_view SECOND_STRV("Second string.");
}

TEST(ee_fwrk_base, strcmp_empty_with_empty) {
    EXPECT_EQ(ee_strcmp("", ""), 0);
}

TEST(ee_fwrk_base, strcmp_valid_with_empty) {
    EXPECT_GT(ee_strcmp(std::string(FIRST_STRV).c_str(), ""), 0);
}

TEST(ee_fwrk_base, strcmp_empty_with_valid) {
    EXPECT_LT(ee_strcmp("", std::string(FIRST_STRV).c_str()), 0);
}

TEST(ee_fwrk_base, strcmp_first_with_first) {
    EXPECT_EQ(ee_strcmp(std::string(FIRST_STRV).c_str(), std::string(FIRST_STRV).c_str()), 0);
}

TEST(ee_fwrk_base, strcmp_second_with_second) {
    EXPECT_EQ(ee_strcmp(std::string(SECOND_STRV).c_str(), std::string(SECOND_STRV).c_str()), 0);
}

TEST(ee_fwrk_base, strcmp_first_with_second) {
    EXPECT_LT(ee_strcmp(std::string(FIRST_STRV).c_str(), std::string(SECOND_STRV).c_str()), 0);
}

TEST(ee_fwrk_base, strcmp_second_with_first) {
    EXPECT_GT(ee_strcmp(std::string(SECOND_STRV).c_str(), std::string(FIRST_STRV).c_str()), 0);
}