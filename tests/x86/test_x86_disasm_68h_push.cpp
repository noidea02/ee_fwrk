#include <array>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    constexpr std::array<ee_byte_t, 5> PUSH_11223344h{ 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 6> OSO_PUSH_11223344h{ 0x66, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 6> ASO_PUSH_11223344h{ 0x67, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 7> OSO_ASO_PUSH_11223344h{ 0x66, 0x67, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 6> REXWRXB_PUSH_11223344h{ 0x4f, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 7> REXWRXB_OSO_PUSH_11223344h{ 0x4f, 0x66, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
    constexpr std::array<ee_byte_t, 7> OSO_REXWRXB_PUSH_11223344h{ 0x66, 0x4f, 0x68, 0x44, 0x33, 0x22, 0x11 }; // push 11223344h
}

TEST(ee_fwrk_x86, disasm_16_push_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, PUSH_11223344h.data(), PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_push_66h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_PUSH_11223344h.data(), OSO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_push_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_PUSH_11223344h.data(), ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_push_66h_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_ASO_PUSH_11223344h.data(), OSO_ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_push_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, PUSH_11223344h.data(), PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_push_66h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_PUSH_11223344h.data(), OSO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_push_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_PUSH_11223344h.data(), ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_push_66h_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_ASO_PUSH_11223344h.data(), OSO_ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, PUSH_11223344h.data(), PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_66h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_PUSH_11223344h.data(), OSO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_PUSH_11223344h.data(), ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_66h_67h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_ASO_PUSH_11223344h.data(), OSO_ASO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_4Fh_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(6, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXWRXB_PUSH_11223344h.data(), REXWRXB_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_4Fh_66h_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(5, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x3344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXWRXB_OSO_PUSH_11223344h.data(), REXWRXB_OSO_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_push_66h_4Fh_68h) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(7, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_REXWRXB_PUSH_11223344h.data(), OSO_REXWRXB_PUSH_11223344h.size(), &is));
    EXPECT_EQ(expected, is);
}