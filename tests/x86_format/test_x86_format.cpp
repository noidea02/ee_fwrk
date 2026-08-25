#include <array>
#include <memory>
#include <span>
#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_x86_format.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    constexpr auto REGULAR_INSTRUCTION{ make_array<ee_byte_t>(0x00, 0x00) };
    constexpr std::string_view REGULAR_INSTRUCTION_SV("add byte ptr[eax], al");

    constexpr auto INSTRUCTION_WITH_REL8_OPD{ make_array<ee_byte_t>(0xeb, 0x02) };
    constexpr auto INSTRUCTION_WITH_66h_REL8_OPD{ make_array<ee_byte_t>(0x66, 0xeb, 0x02) };
    constexpr auto INSTRUCTION_WITH_REL16_OPD{ make_array<ee_byte_t>(0xe9, 0x02, 0x00) };
    constexpr auto INSTRUCTION_WITH_REL32_OPD{ make_array<ee_byte_t>(0xe9, 0x02, 0x00, 0x00, 0x00) };
    constexpr auto INSTRUCTION_WITH_66h_REL32_OPD{ make_array<ee_byte_t>(0x66, 0xe9, 0x02, 0x00, 0x00, 0x00) };
    constexpr std::string_view INSTRUCTION_16_AT_00h_WITH_REL32_OPD_SV("jmp 0005h");
    constexpr std::string_view INSTRUCTION_32_AT_00h_WITH_REL32_OPD_SV("jmp 00000007h");
    constexpr std::string_view INSTRUCTION_64_AT_00h_WITH_REL32_OPD_SV("jmp 0000000000000007h");
    constexpr std::string_view INSTRUCTION_16_AT_7F00h_WITH_REL8_OPD_SV("jmp 7F04h");
    constexpr std::string_view INSTRUCTION_16_AT_7FFF0000h_WITH_66h_REL8_OPD_SV("jmp 7FFF0005h");
    constexpr std::string_view INSTRUCTION_32_AT_7FFF0000h_WITH_REL8_OPD_SV("jmp 7FFF0004h");
    constexpr std::string_view INSTRUCTION_32_AT_7FFF0000h_WITH_66h_REL8_OPD_SV("jmp 0005h");
    constexpr std::string_view INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_REL8_OPD_SV("jmp 7FFFFFFFFFFF0004h");
    constexpr std::string_view INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_66h_REL8_OPD_SV("jmp 7FFFFFFFFFFF0005h");
    constexpr std::string_view INSTRUCTION_16_AT_7F00h_WITH_REL16_OPD_SV("jmp 7F05h");
    constexpr std::string_view INSTRUCTION_16_AT_7FFF0000h_WITH_66h_REL32_OPD_SV("jmp 7FFF0008h");
    constexpr std::string_view INSTRUCTION_32_AT_7FFF0000h_WITH_REL32_OPD_SV("jmp 7FFF0007h");
    constexpr std::string_view INSTRUCTION_32_AT_7FFF0000h_WITH_66h_REL16_OPD_SV("jmp 0006h");
    constexpr std::string_view INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_REL32_OPD_SV("jmp 7FFFFFFFFFFF0007h");
    constexpr std::string_view INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_66h_REL32_OPD_SV("jmp 7FFFFFFFFFFF0008h");

    constexpr auto INSTRUCTION_WITH_RIP_REL_ADDRESSING{ make_array<ee_byte_t>(0x8b, 0x05, 0x00, 0x00, 0x00, 0x00) };
    constexpr auto INSTRUCTION_WITH_67h_RIP_REL_ADDRESSING{ make_array<ee_byte_t>(0x67, 0x8b, 0x05, 0x00, 0x00, 0x00, 0x00) };
    constexpr std::string_view INSTRUCTION_AT_7FFFFFFF00000000h_WITH_RIP_REL_ADDRESSING_SV("mov eax, dword ptr[7FFFFFFF00000006h]");
    constexpr std::string_view INSTRUCTION_AT_7FFFFFFF00000000h_WITH_67h_RIP_REL_ADDRESSING_SV("mov eax, dword ptr[00000007h]");

    constexpr auto INSTRUCTION_WITH_IMM8_OPD{ make_array<ee_byte_t>(0xb0, 0x01) };
    constexpr auto INSTRUCTION_WITH_IMM16_OPD{ make_array<ee_byte_t>(0x66, 0xb8, 0x22, 0x01) };
    constexpr auto INSTRUCTION_WITH_IMM32_OPD{ make_array<ee_byte_t>(0xb8, 0x44, 0x33, 0x02, 0x00) };
    constexpr auto INSTRUCTION_WITH_IMM64_OPD{ make_array<ee_byte_t>(0x48, 0xb8, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x02, 0x00) };
    constexpr std::string_view INSTRUCTION_WITH_IMM8_OPD_SV("mov al, 01h");
    constexpr std::string_view INSTRUCTION_WITH_IMM16_OPD_SV("mov ax, 0122h");
    constexpr std::string_view INSTRUCTION_WITH_IMM32_OPD_SV("mov eax, 023344h");
    constexpr std::string_view INSTRUCTION_WITH_IMM64_OPD_SV("mov rax, 02334455667788h");

    constexpr auto INSTRUCTION_WITH_POSITIVE_MODRM_DISP8_OPD{ make_array<ee_byte_t>(0x8b, 0x43, 0x2f) };
    constexpr auto INSTRUCTION_WITH_NEGATIVE_MODRM_DISP8_OPD{ make_array<ee_byte_t>(0x8b, 0x43, 0xcf) };
    constexpr auto INSTRUCTION_WITH_POSITIVE_MODRM_DISP16_OPD{ make_array<ee_byte_t>(0x67, 0x8b, 0x83, 0x1e, 0x2f) };
    constexpr auto INSTRUCTION_WITH_NEGATIVE_MODRM_DISP16_OPD{ make_array<ee_byte_t>(0x67, 0x8b, 0x83, 0xbe, 0xcf) };
    constexpr auto INSTRUCTION_WITH_POSITIVE_MODRM_DISP32_OPD{ make_array<ee_byte_t>(0x8b, 0x83, 0x0c, 0x0d, 0x1e, 0x2f) };
    constexpr auto INSTRUCTION_WITH_NEGATIVE_MODRM_DISP32_OPD{ make_array<ee_byte_t>(0x8b, 0x83, 0x9c, 0xad, 0xbe, 0xcf) };
    constexpr std::string_view INSTRUCTION_WITH_POSITIVE_MODRM_DISP8_OPD_SV("mov eax, dword ptr[ebx+2Fh]");
    constexpr std::string_view INSTRUCTION_WITH_NEGATIVE_MODRM_DISP8_OPD_SV("mov eax, dword ptr[ebx-31h]");
    constexpr std::string_view INSTRUCTION_WITH_POSITIVE_MODRM_DISP16_OPD_SV("mov eax, dword ptr[bp+di+2F1Eh]");
    constexpr std::string_view INSTRUCTION_WITH_NEGATIVE_MODRM_DISP16_OPD_SV("mov eax, dword ptr[bp+di-3042h]");
    constexpr std::string_view INSTRUCTION_WITH_POSITIVE_MODRM_DISP32_OPD_SV("mov eax, dword ptr[ebx+2F1E0D0Ch]");
    constexpr std::string_view INSTRUCTION_WITH_NEGATIVE_MODRM_DISP32_OPD_SV("mov eax, dword ptr[ebx-30415264h]");

    constexpr auto INSTRUCTION_WITH_THREE_OPDS_RRR{ make_array<ee_byte_t>(0xc5, 0xf1, 0x58, 0xc2) };
    constexpr std::string_view INSTRUCTION_WITH_THREE_OPDS_RRR_SV("vaddpd xmm0, xmm1, xmm2");

    constexpr auto INSTRUCTION_WITH_THREE_OPDS_RRP{ make_array<ee_byte_t>(0xc5, 0xf1, 0x58, 0x00) };
    constexpr std::string_view INSTRUCTION_16_WITH_THREE_OPDS_RRP_SV("vaddpd xmm0, xmm1, xmmword ptr[bx+si]");
    constexpr std::string_view INSTRUCTION_32_WITH_THREE_OPDS_RRP_SV("vaddpd xmm0, xmm1, xmmword ptr[eax]");
    constexpr std::string_view INSTRUCTION_64_WITH_THREE_OPDS_RRP_SV("vaddpd xmm0, xmm1, xmmword ptr[rax]");

    constexpr auto INSTRUCTION_WITH_LOCK_PREFIX{ make_array<ee_byte_t>(0xf0, 0xff, 0x00) };
    constexpr auto INSTRUCTION_WITH_REPNE_PREFIX{ make_array<ee_byte_t>(0xf2, 0xae) };
    constexpr auto INSTRUCTION_WITH_REP_PREFIX{ make_array<ee_byte_t>(0xf3, 0xab) };
    constexpr auto INSTRUCTION_WITH_HT_PREFIX{ make_array<ee_byte_t>(0x3e, 0x74, 0x00) };
    constexpr auto INSTRUCTION_WITH_HNT_PREFIX{ make_array<ee_byte_t>(0x2e, 0x75, 0x00) };
    constexpr auto INSTRUCTION_WITH_XACQUIRE_PREFIX{ make_array<ee_byte_t>(0xf2, 0xf0, 0x0f, 0xb1, 0x08) };
    constexpr auto INSTRUCTION_WITH_XRELEASE_PREFIX{ make_array<ee_byte_t>(0xf3, 0xf0, 0xff, 0x08) };
    constexpr auto INSTRUCTION_WITH_BND_PREFIX{ make_array<ee_byte_t>(0xf2, 0xc3) };
    constexpr std::string_view INSTRUCTION_64_WITH_LOCK_PREFIX_SV("lock inc dword ptr[rax]");
    constexpr std::string_view INSTRUCTION_64_WITH_REPNE_PREFIX_SV("repne scas al, byte ptr[rdi]");
    constexpr std::string_view INSTRUCTION_64_WITH_REP_PREFIX_SV("rep stos dword ptr[rdi], eax");
    constexpr std::string_view INSTRUCTION_64_WITH_HT_PREFIX_SV("ht je 0000000000000003h");
    constexpr std::string_view INSTRUCTION_64_WITH_HNT_PREFIX_SV("hnt jne 0000000000000003h");
    constexpr std::string_view INSTRUCTION_64_WITH_XACQUIRE_PREFIX_SV("lock xacquire cmpxchg dword ptr[rax], ecx");
    constexpr std::string_view INSTRUCTION_64_WITH_XRELEASE_PREFIX_SV("lock xrelease dec dword ptr[rax]");
    constexpr std::string_view INSTRUCTION_64_WITH_BND_PREFIX_SV("bnd ret");

    void expect_format(ee_x86_mode_t mode, std::span<const ee_byte_t> inst, ee_uint64_t addr, std::string_view expected) {

        ee_x86_disasm_output dis_out{};
        ASSERT_TRUE(ee_x86_disasm(mode, inst.data(), inst.size(), &dis_out));

        ee_size_t req_size{ 256 };
        const auto out{ std::make_unique<ee_ascii_char_t[]>(req_size) };

        ASSERT_TRUE(ee_x86_format(mode, addr, &dis_out, out.get(), &req_size));
        EXPECT_STREQ(out.get(), std::string(expected).c_str());
    }
}

TEST(ee_fwrk_x86_format, format_with_nullptr_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    ee_size_t req_size{};
    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, &req_size));
}

TEST(ee_fwrk_x86_format, format_with_zero_len_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    std::array<ee_ascii_char_t, 0> out{};
    ee_size_t req_size{};

    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, out.data(), &req_size));
}

TEST(ee_fwrk_x86_format, format_with_too_small_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    std::array<ee_ascii_char_t, REGULAR_INSTRUCTION.size() / 2> out{};
    ee_size_t req_size{ out.size() };

    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, out.data(), &req_size));
}

TEST(ee_fwrk_x86_format, format_with_barely_big_enough_buf) {
    expect_format(EE_X86_MODE_32, REGULAR_INSTRUCTION, 0, REGULAR_INSTRUCTION_SV);
}

TEST(ee_fwrk_x86_format, format_with_buf_size_correction) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    ee_size_t req_size{};
    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, &req_size));
    EXPECT_EQ(req_size, REGULAR_INSTRUCTION_SV.size() + 1);

    const auto out{ std::make_unique<ee_ascii_char_t[]>(req_size) };
    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, out.get(), &req_size));
    EXPECT_STREQ(out.get(), std::string(REGULAR_INSTRUCTION_SV).c_str());
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel8_16) {
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_REL8_OPD, 0x7f00, INSTRUCTION_16_AT_7F00h_WITH_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel8_16) {
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_66h_REL8_OPD, 0x7fff0000, INSTRUCTION_16_AT_7FFF0000h_WITH_66h_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel8_32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_REL8_OPD, 0x7fff0000, INSTRUCTION_32_AT_7FFF0000h_WITH_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel8_32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_66h_REL8_OPD, 0x7fff0000, INSTRUCTION_32_AT_7FFF0000h_WITH_66h_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel8_64) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_REL8_OPD, 0x7fffffffffff0000, INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel8_64) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_66h_REL8_OPD, 0x7fffffffffff0000, INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_66h_REL8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel16_16) {
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_REL16_OPD, 0x7f00, INSTRUCTION_16_AT_7F00h_WITH_REL16_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel32_16) {
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_66h_REL32_OPD, 0x7fff0000, INSTRUCTION_16_AT_7FFF0000h_WITH_66h_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel32_32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_REL32_OPD, 0x7fff0000, INSTRUCTION_32_AT_7FFF0000h_WITH_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel16_32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_66h_REL32_OPD, 0x7fff0000, INSTRUCTION_32_AT_7FFF0000h_WITH_66h_REL16_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_rel32_64) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_REL32_OPD, 0x7fffffffffff0000, INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_non_zero_inst_address_and_66h_rel32_64) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_66h_REL32_OPD, 0x7fffffffffff0000, INSTRUCTION_64_AT_7FFFFFFFFFFF0000h_WITH_66h_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_rip_relative_addressing) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_RIP_REL_ADDRESSING, 0x7fffffff00000000, INSTRUCTION_AT_7FFFFFFF00000000h_WITH_RIP_REL_ADDRESSING_SV);
}

TEST(ee_fwrk_x86_format, format_with_67h_rip_relative_addressing) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_67h_RIP_REL_ADDRESSING, 0x7fffffff00000000, INSTRUCTION_AT_7FFFFFFF00000000h_WITH_67h_RIP_REL_ADDRESSING_SV);
}

TEST(ee_fwrk_x86_format, format_with_address_padding_16) {
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_REL32_OPD, 0, INSTRUCTION_16_AT_00h_WITH_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_address_padding_32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_REL32_OPD, 0, INSTRUCTION_32_AT_00h_WITH_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_address_padding_64) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_REL32_OPD, 0, INSTRUCTION_64_AT_00h_WITH_REL32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_imm8_value_padding) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_IMM8_OPD, 0, INSTRUCTION_WITH_IMM8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_imm16_value_padding) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_IMM16_OPD, 0, INSTRUCTION_WITH_IMM16_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_imm32_value_padding) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_IMM32_OPD, 0, INSTRUCTION_WITH_IMM32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_imm64_value_padding) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_IMM64_OPD, 0, INSTRUCTION_WITH_IMM64_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_positive_modrm_disp8) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_POSITIVE_MODRM_DISP8_OPD, 0, INSTRUCTION_WITH_POSITIVE_MODRM_DISP8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_negative_modrm_disp8) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP8_OPD, 0, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP8_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_positive_modrm_disp16) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_POSITIVE_MODRM_DISP16_OPD, 0, INSTRUCTION_WITH_POSITIVE_MODRM_DISP16_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_negative_modrm_disp16) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP16_OPD, 0, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP16_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_positive_modrm_disp32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_POSITIVE_MODRM_DISP32_OPD, 0, INSTRUCTION_WITH_POSITIVE_MODRM_DISP32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_negative_modrm_disp32) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP32_OPD, 0, INSTRUCTION_WITH_NEGATIVE_MODRM_DISP32_OPD_SV);
}

TEST(ee_fwrk_x86_format, format_with_three_operands_rrr) {
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_THREE_OPDS_RRR, 0, INSTRUCTION_WITH_THREE_OPDS_RRR_SV);
}

TEST(ee_fwrk_x86_format, format_with_three_operands_rrp) {
    
    expect_format(EE_X86_MODE_16, INSTRUCTION_WITH_THREE_OPDS_RRP, 0, INSTRUCTION_16_WITH_THREE_OPDS_RRP_SV);
    expect_format(EE_X86_MODE_32, INSTRUCTION_WITH_THREE_OPDS_RRP, 0, INSTRUCTION_32_WITH_THREE_OPDS_RRP_SV);
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_THREE_OPDS_RRP, 0, INSTRUCTION_64_WITH_THREE_OPDS_RRP_SV);
}

TEST(ee_fwrk_x86_format, format_with_lock_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_LOCK_PREFIX, 0, INSTRUCTION_64_WITH_LOCK_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_repne_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_REPNE_PREFIX, 0, INSTRUCTION_64_WITH_REPNE_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_rep_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_REP_PREFIX, 0, INSTRUCTION_64_WITH_REP_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_ht_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_HT_PREFIX, 0, INSTRUCTION_64_WITH_HT_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_hnt_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_HNT_PREFIX, 0, INSTRUCTION_64_WITH_HNT_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_xacquire_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_XACQUIRE_PREFIX, 0, INSTRUCTION_64_WITH_XACQUIRE_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_xrelease_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_XRELEASE_PREFIX, 0, INSTRUCTION_64_WITH_XRELEASE_PREFIX_SV);
}

TEST(ee_fwrk_x86_format, format_with_bnd_prefix) {
    expect_format(EE_X86_MODE_64, INSTRUCTION_WITH_BND_PREFIX, 0, INSTRUCTION_64_WITH_BND_PREFIX_SV);
}