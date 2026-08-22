#include <gtest/gtest.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    void expect_empty_prefixes(const ee_x86_disasm_output& out) {

        EXPECT_FALSE(out.prefixes.operand_size_override);
        EXPECT_FALSE(out.prefixes.address_size_override);

        EXPECT_EQ(out.prefixes.num_printable_prefixes, 0);
        EXPECT_EQ(out.prefixes.printable_prefixes[0], EE_X86_PREFIX_NOT_EXISTING);
        EXPECT_EQ(out.prefixes.printable_prefixes[1], EE_X86_PREFIX_NOT_EXISTING);
    }
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_mov_qword_eax_rcx) {

    static constexpr auto out{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RCX) };

    ASSERT_EQ(out.num_instruction_bytes, 2);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_MOV);
    EXPECT_EQ(out.num_operands, 2);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_POINTER);
    EXPECT_EQ(out.operands[0].un.pointer.type, EE_X86_POINTER_QWORD);
    EXPECT_TRUE(out.operands[0].un.pointer.base_register.is_set);
    EXPECT_FALSE(out.operands[0].un.pointer.base_register.is_extended);
    EXPECT_EQ(out.operands[0].un.pointer.base_register.un.reg1ster, EE_X86_GPP_REGISTER_EAX);

    EXPECT_EQ(out.operands[0].un.pointer.displacement, 0);
    EXPECT_EQ(out.operands[0].un.pointer.displacement_num_bits, 0);
    EXPECT_EQ(out.operands[0].un.pointer.scaling, 0);
    EXPECT_EQ(out.operands[0].un.pointer.segment_register, EE_X86_SEGMENT_REGISTER_NOT_EXISTING);
    EXPECT_FALSE(out.operands[0].un.pointer.uses_rip_relative_addressing);

    EXPECT_EQ(out.operands[1].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[1].un.reg1ster.type, EE_X86_REGISTER_GENERAL_PURPOSE);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.general_purpose_register, EE_X86_GPP_REGISTER_RCX);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_mov_qword_eax_ebx_rcx) {

    static constexpr auto out{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX, EE_X86_GPP_REGISTER_EBX }, EE_X86_GPP_REGISTER_RCX) };

    EXPECT_EQ(out.num_instruction_bytes, 2);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_MOV);
    EXPECT_EQ(out.num_operands, 2);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_POINTER);
    EXPECT_EQ(out.operands[0].un.pointer.type, EE_X86_POINTER_QWORD);

    EXPECT_TRUE(out.operands[0].un.pointer.base_register.is_set);
    EXPECT_FALSE(out.operands[0].un.pointer.base_register.is_extended);
    EXPECT_EQ(out.operands[0].un.pointer.base_register.un.reg1ster, EE_X86_GPP_REGISTER_EAX);
    EXPECT_TRUE(out.operands[0].un.pointer.index_register.is_set);
    EXPECT_FALSE(out.operands[0].un.pointer.index_register.is_gpp_ex);
    EXPECT_FALSE(out.operands[0].un.pointer.index_register.is_vec);
    EXPECT_EQ(out.operands[0].un.pointer.index_register.un.general_purpose_register, EE_X86_GPP_REGISTER_EBX);

    EXPECT_EQ(out.operands[0].un.pointer.displacement, 0);
    EXPECT_EQ(out.operands[0].un.pointer.displacement_num_bits, 0);
    EXPECT_EQ(out.operands[0].un.pointer.scaling, 0);
    EXPECT_EQ(out.operands[0].un.pointer.segment_register, EE_X86_SEGMENT_REGISTER_NOT_EXISTING);
    EXPECT_FALSE(out.operands[0].un.pointer.uses_rip_relative_addressing);

    EXPECT_EQ(out.operands[1].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[1].un.reg1ster.type, EE_X86_REGISTER_GENERAL_PURPOSE);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.general_purpose_register, EE_X86_GPP_REGISTER_RCX);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_push_1122h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x1122 }) };

    EXPECT_EQ(out.num_instruction_bytes, 3);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_PUSH);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_IMMEDIATE_VALUE);
    EXPECT_EQ(out.operands[0].un.immediate_value.value, 0x1122);
    EXPECT_EQ(out.operands[0].un.immediate_value.value_num_bits, 16);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_push_11223344h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };

    EXPECT_EQ(out.num_instruction_bytes, 5);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_PUSH);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_IMMEDIATE_VALUE);
    EXPECT_EQ(out.operands[0].un.immediate_value.value, 0x11223344);
    EXPECT_EQ(out.operands[0].un.immediate_value.value_num_bits, 32);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_jmp_1122h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_JMP, x86_rel_opd{ 16, 0x1122 }) };

    EXPECT_EQ(out.num_instruction_bytes, 3);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_JMP);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_RELATIVE_ADDRESS);
    EXPECT_EQ(out.operands[0].un.relative_address.offset, 0x1122);
    EXPECT_EQ(out.operands[0].un.relative_address.offset_num_bits, 16);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_jmp_11223344h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_JMP, x86_rel_opd{ 32, 0x11223344 }) };

    EXPECT_EQ(out.num_instruction_bytes, 5);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_JMP);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_RELATIVE_ADDRESS);
    EXPECT_EQ(out.operands[0].un.relative_address.offset, 0x11223344);
    EXPECT_EQ(out.operands[0].un.relative_address.offset_num_bits, 32);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_call_1122h_3344h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 16, 0x3344 }) };

    EXPECT_EQ(out.num_instruction_bytes, 5);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_CALL);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_FAR_POINTER);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_selector, 0x1122);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_offset_num_bits, 16);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_offset, 0x3344);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_call_1122h_33445566h) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(7, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };

    EXPECT_EQ(out.num_instruction_bytes, 7);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_CALL);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_FAR_POINTER);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_selector, 0x1122);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_offset_num_bits, 32);
    EXPECT_EQ(out.operands[0].un.far_pointer.cs_offset, 0x33445566);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_inc_eax) {

    static constexpr auto out{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    EXPECT_EQ(out.num_instruction_bytes, 2);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_INC);
    EXPECT_EQ(out.num_operands, 1);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[0].un.reg1ster.type, EE_X86_REGISTER_GENERAL_PURPOSE);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.general_purpose_register, EE_X86_GPP_REGISTER_EAX);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_vaddpd_xmm0_xmm1_xmm2) {

    static constexpr auto out{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    EXPECT_EQ(out.num_instruction_bytes, 4);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_VADDPD);
    EXPECT_EQ(out.num_operands, 3);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[0].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.index, 0);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[1].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[1].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.index, 1);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[2].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[2].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[2].un.reg1ster.un.numbered_register.index, 2);
    EXPECT_EQ(out.operands[2].un.reg1ster.un.numbered_register.size_bits, 128);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_vaddpd_xmm0_xmm1_xmmword_eax) {

    static constexpr auto out{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd{ EE_X86_POINTER_XMMWORD, EE_X86_GPP_REGISTER_EAX }) };

    EXPECT_EQ(out.num_instruction_bytes, 4);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_VADDPD);
    EXPECT_EQ(out.num_operands, 3);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[0].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.index, 0);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[1].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[1].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.index, 1);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[2].type, EE_X86_OPERAND_POINTER);
    EXPECT_EQ(out.operands[2].un.pointer.type, EE_X86_POINTER_XMMWORD);
    EXPECT_TRUE(out.operands[2].un.pointer.base_register.is_set);
    EXPECT_FALSE(out.operands[2].un.pointer.base_register.is_extended);
    EXPECT_EQ(out.operands[2].un.pointer.base_register.un.reg1ster, EE_X86_GPP_REGISTER_EAX);
}

TEST(ee_fwrk_x86, sanity_check_create_x86_output_vaddpd_xmm0_xmm1_xmmword_r8) {

    static constexpr auto out{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd_with_extended_base{ EE_X86_POINTER_XMMWORD, 8, 64 }) };

    EXPECT_EQ(out.num_instruction_bytes, 4);
    expect_empty_prefixes(out);

    EXPECT_EQ(out.instruction, EE_X86_INSTRUCTION_VADDPD);
    EXPECT_EQ(out.num_operands, 3);

    EXPECT_EQ(out.operands[0].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[0].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.index, 0);
    EXPECT_EQ(out.operands[0].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[1].type, EE_X86_OPERAND_REGISTER);
    EXPECT_EQ(out.operands[1].un.reg1ster.type, EE_X86_REGISTER_AVX_128);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.index, 1);
    EXPECT_EQ(out.operands[1].un.reg1ster.un.numbered_register.size_bits, 128);

    EXPECT_EQ(out.operands[2].type, EE_X86_OPERAND_POINTER);
    EXPECT_EQ(out.operands[2].un.pointer.type, EE_X86_POINTER_XMMWORD);
    EXPECT_TRUE(out.operands[2].un.pointer.base_register.is_set);
    EXPECT_TRUE(out.operands[2].un.pointer.base_register.is_extended);
    EXPECT_EQ(out.operands[2].un.pointer.base_register.un.ex_register.index, 8);
    EXPECT_EQ(out.operands[2].un.pointer.base_register.un.ex_register.size_bits, 64);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_empty) {

    ee_x86_disasm_output out0{}, out1{};
    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_non_empty) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RCX) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_index_reg) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX, EE_X86_GPP_REGISTER_EBX }, EE_X86_GPP_REGISTER_RCX) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_imm16_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x1122 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_imm32_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 32, 0x11223344 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_rel16_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_rel_opd{ 16, 0x1122 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_rel32_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_rel_opd{ 32, 0x11223344 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_farptr16_16_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 16, 0x3344 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_farptr16_32_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(7, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 32, 0x33445566 }) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_gpp_register_opd) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };
    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_xmm0_xmm1_xmm2_opds) {

    static constexpr auto out0{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_xmm0_xmm1_xmmword_eax_opds) {

    static constexpr auto out0{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd{ EE_X86_POINTER_XMMWORD, EE_X86_GPP_REGISTER_EAX }) };

    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_positive_with_xmm0_xmm1_xmmword_r8_opds) {

    static constexpr auto out0{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd_with_extended_base{ EE_X86_POINTER_XMMWORD, 8, 128 }) };

    static constexpr auto out1{ out0 };

    EXPECT_EQ(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_empty) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };
    ee_x86_disasm_output out1{};

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_non_empty) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_EAX }, EE_X86_GPP_REGISTER_RCX) };
    static constexpr auto out1{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_inst_len_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    auto out1{ out0 };
    out1.num_instruction_bytes = 4;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_inst_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    auto out1{ out0 };
    out1.instruction = EE_X86_INSTRUCTION_SYSENTER;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_opd_count_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    auto out1{ out0 };
    out1.num_operands = 3;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_scaling_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    auto out1{ out0 };
    out1.operands[0].un.pointer.scaling = 4;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_rip_reladdr_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_two_operands(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_MOV, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }, EE_X86_GPP_REGISTER_ECX) };

    auto out1{ out0 };
    out1.operands[0].un.pointer.uses_rip_relative_addressing = EE_TRUE;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_imm_opd_bits_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x1122 }) };

    auto out1{ out0 };
    out1.operands[0].un.immediate_value.value_num_bits = 32;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_imm_opd_value_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_imm_opd{ 16, 0x1122 }) };

    auto out1{ out0 };
    out1.operands[0].un.immediate_value.value = 0x3344;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_rel_opd_bits_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_rel_opd{ 16, 0x1122 }) };

    auto out1{ out0 };
    out1.operands[0].un.relative_address.offset_num_bits = 32;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_rel_opd_offset_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_PUSH, x86_rel_opd{ 16, 0x1122 }) };

    auto out1{ out0 };
    out1.operands[0].un.relative_address.offset = 0x3344;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_farptr_opd_selector_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 16, 0x3344 }) };

    auto out1{ out0 };
    out1.operands[0].un.far_pointer.cs_selector = 0x3344;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_farptr_opd_bits_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 16, 0x3344 }) };

    auto out1{ out0 };
    out1.operands[0].un.far_pointer.cs_offset_num_bits = 32;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_farptr_opd_offset_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_CALL, x86_farptr_opd{ 0x1122, 16, 0x3344 }) };

    auto out1{ out0 };
    out1.operands[0].un.far_pointer.cs_offset = 0x1122;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_register_type_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    auto out1{ out0 };
    out1.operands[0].un.reg1ster.type = EE_X86_REGISTER_AVX_128;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_gpp_register_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    auto out1{ out0 };
    out1.operands[0].un.reg1ster.un.general_purpose_register = EE_X86_GPP_REGISTER_EBX;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_numbered_register_size_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    auto out1{ out0 };
    out1.operands[1].un.reg1ster.un.numbered_register.size_bits = static_cast<ee_uint8_t>(256);

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_numbered_register_index_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    auto out1{ out0 };
    out1.operands[2].un.reg1ster.un.numbered_register.index = 0;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_operand_size_override_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    auto out1{ out0 };
    out1.prefixes.operand_size_override = EE_TRUE;

    EXPECT_NE(out0, out1);
}

TEST(ee_fwrk_x86, sanity_check_compare_x86_output_negative_with_address_size_override_mismatch) {

    static constexpr auto out0{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    auto out1{ out0 };
    out1.prefixes.address_size_override = EE_TRUE;

    EXPECT_NE(out0, out1);
}