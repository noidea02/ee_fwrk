#include <array>
#include <span>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include <ee_support/utils.hpp>

#include "test_x86_disasm_utils.hpp"

using namespace ee_supp;
using namespace ee_test;

namespace {
    
    constexpr auto INSTRUCTIONS{ make_array<ee_byte_t>(
        0x55,
        0x68, 0x04, 0x03, 0x02, 0x01,
        0x01, 0x03,
        0x01, 0x29,
        0xe8, 0x04, 0x03, 0x02, 0x01,
        0x66, 0x01, 0x03,
        0xe9, 0x04, 0x03, 0x02, 0x01,
        0x67, 0x48, 0x01, 0x00
    ) };

    constexpr auto EXPECTED_16{ make_array<ee_x86_disasm_output>(
        create_x86_disasm_output_with_one_operand(1, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, EE_X86_GPP_REGISTER_BP),
        create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x0304 }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, EE_X86_GPP_REGISTER_AL, x86_ptr_opd{ EE_X86_POINTER_BYTE, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_DI }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BP, EE_X86_GPP_REGISTER_DI }, EE_X86_GPP_REGISTER_AX),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_DI }, EE_X86_GPP_REGISTER_BP),
        create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_rel_opd{ 16, 0x0304 }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, EE_X86_GPP_REGISTER_AL, x86_ptr_opd{ EE_X86_POINTER_BYTE, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_DI }),
        create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BP, EE_X86_GPP_REGISTER_DI }, EE_X86_GPP_REGISTER_EAX),
        create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_JMP, x86_rel_opd{ 16, 0x0304 }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, EE_X86_GPP_REGISTER_AL, x86_ptr_opd{ EE_X86_POINTER_BYTE, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_DI }),
        create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_DEC, EE_X86_GPP_REGISTER_AX),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }, EE_X86_GPP_REGISTER_AX)
    ) };

    constexpr auto EXPECTED_32{ make_array<ee_x86_disasm_output>(
        create_x86_disasm_output_with_one_operand(1, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, EE_X86_GPP_REGISTER_EBP),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EBX }, EE_X86_GPP_REGISTER_EAX),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_ECX }, EE_X86_GPP_REGISTER_EBP),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_rel_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_EBX }, EE_X86_GPP_REGISTER_AX),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_JMP, x86_rel_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_DEC, EE_X86_GPP_REGISTER_EAX),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_EAX)
    ) };

    constexpr auto EXPECTED_64{ make_array<ee_x86_disasm_output>(
        create_x86_disasm_output_with_one_operand(1, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, EE_X86_GPP_REGISTER_RBP),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RBX }, EE_X86_GPP_REGISTER_EAX),
        create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RCX }, EE_X86_GPP_REGISTER_EBP),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_rel_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_two_operands(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_WORD, EE_X86_GPP_REGISTER_RBX }, EE_X86_GPP_REGISTER_AX),
        create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_JMP, x86_rel_opd{ 32, 0x01020304 }),
        create_x86_disasm_output_with_two_operands(4, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADD, x86_ptr_opd{ EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RAX)
    ) };

    void test_disasm_chain(ee_x86_mode_t mode, std::span<const ee_byte_t> chain, std::span<const ee_x86_disasm_output> expected) {

        size_t expected_index{};
        for (ee_size_t byte_index{}; byte_index != chain.size();) {

            ASSERT_LT(byte_index, chain.size());

            ee_x86_disasm_output out{};
            ASSERT_TRUE(ee_x86_disasm(mode, &chain[byte_index], chain.size() - byte_index, &out));

            ASSERT_LT(expected_index, expected.size());
            const ee_x86_disasm_output& expected_out{ expected[expected_index++] };

            ASSERT_EQ(out, expected_out);
            byte_index += out.num_instruction_bytes;
        }
    }
}

TEST(ee_fwrk_x86, disasm_16_instruction_chain) {
    test_disasm_chain(EE_X86_MODE_16, INSTRUCTIONS, EXPECTED_16);
}

TEST(ee_fwrk_x86, disasm_32_instruction_chain) {
    test_disasm_chain(EE_X86_MODE_32, INSTRUCTIONS, EXPECTED_32);
}

TEST(ee_fwrk_x86, disasm_64_instruction_chain) {
    test_disasm_chain(EE_X86_MODE_64, INSTRUCTIONS, EXPECTED_64);
}