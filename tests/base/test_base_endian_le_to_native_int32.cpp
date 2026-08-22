#include <bit>
#include <limits>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include "test_base_endian_conversion_templs.hpp"

using namespace ee_test;

TEST(ee_fwrk_base, endian_le_to_native_int32_zero) {

    EXPECT_EQ(ee_endian_le_to_native_int32(0), 0);
}

TEST(ee_fwrk_base, endian_le_to_native_int32_max_int32) {

    test_base_endian_conversion<std::endian::little>(
        ee_endian_le_to_native_int32, std::numeric_limits<ee_int32_t>::max());
}

TEST(ee_fwrk_base, endian_le_to_native_int32_max_uint32) {

    test_base_endian_conversion<std::endian::little, ee_int32_t>(
        ee_endian_le_to_native_int32, std::numeric_limits<ee_uint32_t>::max());
}