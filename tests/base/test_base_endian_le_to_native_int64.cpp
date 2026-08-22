#include <bit>
#include <limits>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

#include "test_base_endian_conversion_templs.hpp"

using namespace ee_test;

TEST(ee_fwrk_base, endian_le_to_native_int64_zero) {

    EXPECT_EQ(ee_endian_le_to_native_int64(0), 0);
}

TEST(ee_fwrk_base, endian_le_to_native_int64_max_int64) {

    test_base_endian_conversion<std::endian::little>(
        ee_endian_le_to_native_int64, std::numeric_limits<ee_int64_t>::max());
}

TEST(ee_fwrk_base, endian_le_to_native_int64_max_uint64) {

    test_base_endian_conversion<std::endian::little, ee_int64_t>(
        ee_endian_le_to_native_int64, std::numeric_limits<ee_uint64_t>::max());
}