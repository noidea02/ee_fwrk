#include <bit>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

TEST(ee_fwrk_base, endian_platform_is_le) {

    if constexpr (std::endian::native == std::endian::little)
        EXPECT_TRUE(ee_endian_platform_is_le());
    else
        EXPECT_FALSE(ee_endian_platform_is_le());
}