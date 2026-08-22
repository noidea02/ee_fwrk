#include <array>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    constexpr std::array<ee_byte_t, 2> ADD_DWORD_PTR_EAX_EAX{ 0x01, 0x00 }; // add dword ptr[eax], eax
    constexpr std::array<ee_byte_t, 3> OSO_ADD_DWORD_PTR_EAX_EAX{ 0x66, 0x01, 0x00 }; // add dword ptr[eax], eax
    constexpr std::array<ee_byte_t, 3> ASO_ADD_DWORD_PTR_EAX_EAX{ 0x67, 0x01, 0x00 }; // add dword ptr[eax], eax
    constexpr std::array<ee_byte_t, 4> OSO_ASO_ADD_DWORD_PTR_EAX_EAX{ 0x66, 0x67, 0x01, 0x00 }; // add dword ptr[eax], eax
    constexpr std::array<ee_byte_t, 4> ASO_OSO_ADD_DWORD_PTR_EAX_EAX{ 0x67, 0x66, 0x01, 0x00 }; // add dword ptr[eax], eax
    constexpr std::array<ee_byte_t, 3> REXW_ADD_QWORD_PTR_RAX_RAX{ 0x48, 0x01, 0x00 }; // add qword ptr[rax], rax
    constexpr std::array<ee_byte_t, 4> OSO_REXW_ADD_QWORD_PTR_RAX_RAX{ 0x66, 0x48, 0x01, 0x00 }; // add qword ptr[rax], rax
    constexpr std::array<ee_byte_t, 4> ASO_REXW_ADD_QWORD_PTR_EAX_RAX{ 0x67, 0x48, 0x01, 0x00 }; // add qword ptr[eax], rax
    constexpr std::array<ee_byte_t, 5> OSO_ASO_REXW_ADD_QWORD_PTR_EAX_RAX{ 0x66, 0x67, 0x48, 0x01, 0x00 }; // add qword ptr[eax], rax
    constexpr std::array<ee_byte_t, 5> ASO_OSO_REXW_ADD_QWORD_PTR_EAX_RAX{ 0x67, 0x66, 0x48, 0x01, 0x00 }; // add qword ptr[eax], rax
    constexpr std::array<ee_byte_t, 5> REXW_OSO_ASO_ADD_QWORD_PTR_EAX_RAX{ 0x48, 0x66, 0x67, 0x01, 0x00 }; // REX.W add word ptr[eax], ax
    constexpr std::array<ee_byte_t, 5> REXW_ASO_OSO_ADD_QWORD_PTR_EAX_RAX{ 0x48, 0x67, 0x66, 0x01, 0x00 }; // REX.W add word ptr[eax], ax
}

TEST(ee_fwrk_x86, disasm_16_add_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ADD_DWORD_PTR_EAX_EAX.data(), ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_16_add_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_16_add_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_16_add_66h_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_ASO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_16_add_67h_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_OSO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_add_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ADD_DWORD_PTR_EAX_EAX.data(), ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_add_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_add_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_add_66h_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_ASO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_add_67h_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_OSO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ADD_DWORD_PTR_EAX_EAX.data(), ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_66h_67h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_ASO_ADD_DWORD_PTR_EAX_EAX.data(), OSO_ASO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_67h_66h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_OSO_ADD_DWORD_PTR_EAX_EAX.data(), ASO_OSO_ADD_DWORD_PTR_EAX_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_48h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_ADD_QWORD_PTR_RAX_RAX.data(), REXW_ADD_QWORD_PTR_RAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_66h_48h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_REXW_ADD_QWORD_PTR_RAX_RAX.data(), OSO_REXW_ADD_QWORD_PTR_RAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_67h_48h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(4, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_REXW_ADD_QWORD_PTR_EAX_RAX.data(), ASO_REXW_ADD_QWORD_PTR_EAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_66h_67h_48h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_ASO_REXW_ADD_QWORD_PTR_EAX_RAX.data(), OSO_ASO_REXW_ADD_QWORD_PTR_EAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_67h_66h_48h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_OSO_REXW_ADD_QWORD_PTR_EAX_RAX.data(), ASO_OSO_REXW_ADD_QWORD_PTR_EAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_48h_66h_67h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_OSO_ASO_ADD_QWORD_PTR_EAX_RAX.data(), REXW_OSO_ASO_ADD_QWORD_PTR_EAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_add_48h_67h_66h_01h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_ADD,
        { EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_ASO_OSO_ADD_QWORD_PTR_EAX_RAX.data(), REXW_ASO_OSO_ADD_QWORD_PTR_EAX_RAX.size(), &out));
    EXPECT_EQ(expected, out);
}