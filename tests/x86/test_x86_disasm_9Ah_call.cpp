#include <array>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    constexpr std::array<ee_byte_t, 7> CALL_11223344h{ 0x9a, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // JMP 1122h:33445566h
    constexpr std::array<ee_byte_t, 8> OSO_CALL_11223344h{ 0x66, 0x9a, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // JMP 1122h:33445566h
    constexpr std::array<ee_byte_t, 8> ASO_CALL_11223344h{ 0x67, 0x9a, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // JMP 1122h:33445566h
    constexpr std::array<ee_byte_t, 9> OSO_ASO_CALL_11223344h{ 0x66, 0x67, 0x9a, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // JMP 1122h:33445566h
    constexpr std::array<ee_byte_t, 9> ASO_OSO_CALL_11223344h{ 0x66, 0x67, 0x9a, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 }; // JMP 1122h:33445566h
}

TEST(ee_fwrk_x86, disasm_16_call_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x3344, 16, 0x5566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, CALL_11223344h.data(), CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_call_66h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(8, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_CALL_11223344h.data(), OSO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_call_67h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x3344, 16, 0x5566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_CALL_11223344h.data(), ASO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_call_66h_67h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(9, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_ASO_CALL_11223344h.data(), OSO_ASO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_call_67h_66h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(9, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_OSO_CALL_11223344h.data(), ASO_OSO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_call_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, CALL_11223344h.data(), CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_call_66h_9Ah) {
    
    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x3344, 16, 0x5566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_CALL_11223344h.data(), OSO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_call_67h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(8, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_CALL_11223344h.data(), ASO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_call_66h_67h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x3344, 16, 0x5566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_ASO_CALL_11223344h.data(), OSO_ASO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_call_67h_66h_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x3344, 16, 0x5566 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_OSO_CALL_11223344h.data(), ASO_OSO_CALL_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_call_9Ah) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    ee_x86_disasm_output is{};
    EXPECT_FALSE(ee_x86_disasm(EE_X86_MODE_64, CALL_11223344h.data(), CALL_11223344h.size(), &is));
    EXPECT_NE(expected, is);
}