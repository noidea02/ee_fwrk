#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

TEST(ee_fwrk_base, strncmp_empty_with_empty_and_null_num) {
    EXPECT_EQ(ee_strncmp("", "", 0), 0);
}

TEST(ee_fwrk_base, strncmp_empty_with_empty_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("", "", 32), 0);
}

TEST(ee_fwrk_base, strncmp_non_empty_with_empty_and_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty", "", 0), 0);
}

TEST(ee_fwrk_base, strncmp_non_empty_with_empty_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty", "", 32), 1);
}

TEST(ee_fwrk_base, strncmp_empty_with_non_empty_and_null_num) {
    EXPECT_EQ(ee_strncmp("", "Non-Empty", 0), 0);
}

TEST(ee_fwrk_base, strncmp_empty_with_non_empty_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("", "Non-Empty", 32), -1);
}

TEST(ee_fwrk_base, strncmp_non_empty_0_with_non_empty_1_and_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #0", "Non-Empty #1", 0), 0);
}

TEST(ee_fwrk_base, strncmp_non_empty_0_with_non_empty_1_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #0", "Non-Empty #1", 32), -1);
}

TEST(ee_fwrk_base, strncmp_non_empty_1_with_non_empty_0_and_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #1", "Non-Empty #0", 0), 0);
}

TEST(ee_fwrk_base, strncmp_non_empty_1_with_non_empty_0_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #1", "Non-Empty #0", 32), 1);
}

TEST(ee_fwrk_base, strncmp_non_empty_1_with_non_empty_0_and_equal_range_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #1", "Non-Empty #0", 11), 0);
}

TEST(ee_fwrk_base, strncmp_non_empty_1_with_non_empty_0_and_barely_unequal_range_num) {
    EXPECT_EQ(ee_strncmp("Non-Empty #1", "Non-Empty #0", 12), 1);
}

TEST(ee_fwrk_base, strncmp_abc_with_abcdef_and_null_num) {
    EXPECT_EQ(ee_strncmp("abc", "abcdef", 0), 0);
}

TEST(ee_fwrk_base, strncmp_abc_with_abcdef_and_non_null_num) {
    EXPECT_EQ(ee_strncmp("abc", "abcdef", 32), -1);
}

TEST(ee_fwrk_base, strncmp_abc_with_abcdef_and_equal_range_num) {
    EXPECT_EQ(ee_strncmp("abc", "abcdef", 3), 0);
}

TEST(ee_fwrk_base, strncmp_abc_with_abcdef_and_barely_unequal_range_num) {
    EXPECT_EQ(ee_strncmp("abc", "abcdef", 4), -1);
}