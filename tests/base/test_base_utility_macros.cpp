#include <stdint.h>

#include <gtest/gtest.h>

#include <ee/ee_base.h>

namespace {

    typedef struct {

        uint32_t field0, field1, field2;

    } some_tdef_struct0_t;

    typedef struct {

        uint8_t field0;
        uint16_t field1;
        uint32_t field2;
        uint64_t field3;
        some_tdef_struct0_t field4;

    } some_tdef_struct1_t;
}

TEST(ee_fwrk_base, get_array_len) {

    char char_array_1[]{ 0 };
    EXPECT_EQ(EE_GET_ARRAY_LEN(char_array_1), 1);

    char char_array_2[]{ 0, 1 };
    EXPECT_EQ(EE_GET_ARRAY_LEN(char_array_2), 2);

    char char_array_3[]{ 0, 1, 2 };
    EXPECT_EQ(EE_GET_ARRAY_LEN(char_array_3), 3);
}

TEST(ee_fwrk_base, get_tdef_struct_field_len) {

    EXPECT_EQ(EE_GET_TDEF_STRUCT_FIELD_LEN(some_tdef_struct1_t, field0), sizeof(some_tdef_struct1_t::field0));
    EXPECT_EQ(EE_GET_TDEF_STRUCT_FIELD_LEN(some_tdef_struct1_t, field1), sizeof(some_tdef_struct1_t::field1));
    EXPECT_EQ(EE_GET_TDEF_STRUCT_FIELD_LEN(some_tdef_struct1_t, field2), sizeof(some_tdef_struct1_t::field2));
    EXPECT_EQ(EE_GET_TDEF_STRUCT_FIELD_LEN(some_tdef_struct1_t, field3), sizeof(some_tdef_struct1_t::field3));
    EXPECT_EQ(EE_GET_TDEF_STRUCT_FIELD_LEN(some_tdef_struct1_t, field4), sizeof(some_tdef_struct1_t::field4));
}