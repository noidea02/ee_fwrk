#include <bit>
#include <limits>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include "test_base_endian_conversion_templs.hpp"

using namespace ee_test;

TEST(ee_fwrk_base, endian_native_to_le_int16_zero) {

    EXPECT_EQ(ee_endian_native_to_le_int16(0), 0);
}

TEST(ee_fwrk_base, endian_native_to_le_int16_max_int16) {

    test_base_endian_conversion<std::endian::little>(
        ee_endian_native_to_le_int16, std::numeric_limits<ee_int16_t>::max());
}

TEST(ee_fwrk_base, endian_native_to_le_int16_max_uint16) {

    test_base_endian_conversion<std::endian::little, ee_int16_t>(
        ee_endian_native_to_le_int16, std::numeric_limits<ee_uint16_t>::max());
}