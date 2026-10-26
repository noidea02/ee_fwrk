#include <algorithm>
#include <array>
#include <compare>
#include <functional>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include <gtest/gtest.h>

#include <ee/ee_x86_format.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

#define MAX_LEN_MODULE_NAME_FOR_3_DIGIT_OFFS "abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_abcdefghijklmnopqrstuvwxyz_abcdefghi"

namespace {

    struct symbolization_test_context {

        bool called_before;
        bool success;
        bool expected_is_address;
        ee_uint64_t expected_address;
    };

    struct symbolized_address_info {

        bool is_address;
        ee_uint64_t address;

        symbolized_address_info() : is_address{}, address{} {}
        symbolized_address_info(bool is_address, ee_uint64_t address) : is_address{is_address}, address{address} {}

        auto operator<=>(const symbolized_address_info&) const = default;
    };

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

        ASSERT_TRUE(ee_x86_format(mode, addr, &dis_out, nullptr, out.get(), &req_size));
        EXPECT_STREQ(out.get(), std::string(expected).c_str());
    }

    template<bool expect_is_address, bool use_64bit_address_range>
    ee_bool_t check_symbolize_unknown_module_address(void* user_ctx, ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size) {

        if ((expect_is_address != (bool)is_address) || !symbol || !symbol_size)
            return EE_FALSE;

        const ee_uint64_t module_base{ use_64bit_address_range ? 0x7F0000000000 : 0x7F0000 };
        const ee_uint64_t module_end{ use_64bit_address_range ? 0x7FFFFFFFFFFF : 0x7FFFFF };

        if (address < module_base || address >= module_end)
            return EE_FALSE;

        const auto offs{ address - module_base };
        const auto symbol_str{ std::format("UnknownModule+{:X}h", offs) };

        strncpy(symbol, symbol_str.c_str(), symbol_size - 1);
        return EE_TRUE;
    }

    ee_bool_t symbolize_address_with_test_context(void* user_ctx, ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size) {

        const auto test_ctx{ (symbolization_test_context*)user_ctx };
        if(test_ctx->called_before)
            return EE_FALSE;

        test_ctx->called_before = true;
        test_ctx->success = false;

        if(test_ctx->expected_is_address != (bool)is_address)
            return EE_FALSE;

        test_ctx->success = test_ctx->expected_address == address;
        return EE_FALSE;
    }

    bool expect_computed_address(ee_x86_mode_t mode, std::span<const ee_byte_t> inst, ee_uint64_t inst_addr, ee_uint64_t expected_computed_addr, ee_bool_t expected_is_addr) {

        ee_x86_disasm_output dis_out{};
        if(!ee_x86_disasm(mode, inst.data(), inst.size(), &dis_out))
            return false;

        symbolization_test_context symbolization_ctx{};
        symbolization_ctx.expected_is_address = expected_is_addr;
        symbolization_ctx.expected_address = expected_computed_addr;

        ee_x86_symbolization_params_t symbolization_params{};
        symbolization_params.user_ctx = &symbolization_ctx;
        symbolization_params.symbolize_address = symbolize_address_with_test_context;

        ee_ascii_char_t format[128]{};
        ee_size_t format_size{ sizeof(format) };

        if(!ee_x86_format(mode, inst_addr, &dis_out, &symbolization_params, format, &format_size))
            return false;

        return symbolization_ctx.success;
    }

    ee_bool_t symbolize_and_collect_address(void* user_ctx, ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size) {

        const auto address_info_vec{ (std::vector<symbolized_address_info>*)user_ctx };
        address_info_vec->push_back(symbolized_address_info(is_address, address));

        return EE_FALSE;
    }

    bool expect_computed_addresses(ee_x86_mode_t mode, std::span<const ee_byte_t> inst, ee_uint64_t inst_addr, std::span<const symbolized_address_info> expected_computed_addresses) {

        ee_x86_disasm_output dis_out{};
        if(!ee_x86_disasm(mode, inst.data(), inst.size(), &dis_out))
            return false;

        std::vector<symbolized_address_info> computed_addresses;

        ee_x86_symbolization_params_t symbolization_params{};
        symbolization_params.user_ctx = &computed_addresses;
        symbolization_params.symbolize_address = symbolize_and_collect_address;

        ee_ascii_char_t format[128]{};
        ee_size_t format_size{ sizeof(format) };

        if(!ee_x86_format(mode, inst_addr, &dis_out, &symbolization_params, format, &format_size))
            return false;

        for(const auto& expected_computed_address : expected_computed_addresses) {

            if(std::find(computed_addresses.begin(), computed_addresses.end(), expected_computed_address) == computed_addresses.end())
                return false;
        }

        return true;
    }
}

TEST(ee_fwrk_x86_format, format_with_nullptr_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    ee_size_t req_size{};
    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, nullptr, &req_size));
}

TEST(ee_fwrk_x86_format, format_with_nullptr_buf_but_correct_size) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    ee_size_t req_size{ 256 };
    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, nullptr, &req_size));
}

TEST(ee_fwrk_x86_format, format_with_zero_len_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    std::array<ee_ascii_char_t, 0> out{};
    ee_size_t req_size{};

    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, out.data(), &req_size));
}

TEST(ee_fwrk_x86_format, format_with_too_small_buf) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    std::array<ee_ascii_char_t, REGULAR_INSTRUCTION.size() / 2> out{};
    ee_size_t req_size{ out.size() };

    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, out.data(), &req_size));
}

TEST(ee_fwrk_x86_format, format_with_barely_big_enough_buf) {
    expect_format(EE_X86_MODE_32, REGULAR_INSTRUCTION, 0, REGULAR_INSTRUCTION_SV);
}

TEST(ee_fwrk_x86_format, format_with_buf_size_correction) {

    ee_x86_disasm_output dis_out{};
    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, REGULAR_INSTRUCTION.data(), REGULAR_INSTRUCTION.size(), &dis_out));

    ee_size_t req_size{};
    EXPECT_FALSE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, nullptr, &req_size));
    EXPECT_EQ(req_size, REGULAR_INSTRUCTION_SV.size() + 1);

    const auto out{ std::make_unique<ee_ascii_char_t[]>(req_size) };
    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_32, 0, &dis_out, nullptr, out.get(), &req_size));
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

TEST(ee_fwrk_x86_format, format_with_relative_address_symbolization) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE8, 0x50, 0x00, 0x00, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<true, false>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "call UnknownModule+100h");
}

TEST(ee_fwrk_x86_format, format_with_relative_address_symbolization_max_len) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE8, 0x75, 0x00, 0x00, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    constexpr static auto symbolize_addr{ [](void* user_ctx, ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size) {

        if (!is_address || !symbol || !symbol_size)
            return EE_FALSE;

        const ee_uint64_t module_base{ 0x7F0000 };
        if (module_base > address)
            return EE_FALSE;

        const auto offs{ address - module_base };
        const auto symbol_str{ std::format(MAX_LEN_MODULE_NAME_FOR_3_DIGIT_OFFS "+{:X}h", offs) };

        strncpy(symbol, symbol_str.c_str(), symbol_size - 1);
        return EE_TRUE;

    } };

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = symbolize_addr;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00BB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "call " MAX_LEN_MODULE_NAME_FOR_3_DIGIT_OFFS "+135h");
}

TEST(ee_fwrk_x86_format, format_with_imm32_symbolization) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xB8, 0xCD, 0xAB, 0x7F, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<false, false>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "mov eax, UnknownModule+ABCDh");
}

TEST(ee_fwrk_x86_format, format_with_imm32_failed_symbolization) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xB8, 0x80, 0x00, 0x80, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<false, false>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "mov eax, 800080h");
}

TEST(ee_fwrk_x86_format, format_with_imm64_symbolization) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x48, 0xB8, 0xBB, 0x00, 0x00, 0x00, 0xAA, 0x7F, 0x00, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<false, true>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00000000AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "mov rax, UnknownModule+AA000000BBh");
}

TEST(ee_fwrk_x86_format, format_with_modrm_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x05, 0xEE, 0x00, 0x7F, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_32, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<true, false>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_32, 0x7F00AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "mov eax, dword ptr[UnknownModule+EEh]");
}

TEST(ee_fwrk_x86_format, format_with_modrm_absolute_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x48, 0x8B, 0x05, 0xFF, 0x00, 0x00, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.symbolize_address = check_symbolize_unknown_module_address<true, true>;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0x7F00000000AB, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_STREQ(format, "mov rax, qword ptr[UnknownModule+1B1h]");
}

TEST(ee_fwrk_x86_format, format_with_symbolisation_user_ctx) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE8, 0x75, 0x00, 0x00, 0x00) };
    ee_x86_disasm_output ast_out{};

    ASSERT_TRUE(ee_x86_disasm(EE_X86_MODE_64, inst.data(), inst.size(), &ast_out));

    constexpr auto expected_user_ctx_value{ 0x12AB34CD };
    constexpr static auto symbolize_addr{ [](void* user_ctx, ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size) {
        
        *((int*)user_ctx) = expected_user_ctx_value;
        return EE_FALSE;

    } };

    int user_ctx{};
    ee_x86_symbolization_params_t symbolization_params{};
    symbolization_params.user_ctx = &user_ctx;
    symbolization_params.symbolize_address = symbolize_addr;

    ee_ascii_char_t format[128]{};
    ee_size_t format_size{ sizeof(format) };

    ASSERT_TRUE(ee_x86_format(EE_X86_MODE_64, 0, &ast_out, &symbolization_params, format, &format_size));
    ASSERT_EQ(user_ctx, expected_user_ctx_value);
}

TEST(ee_fwrk_x86_format, format_with_imm32_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xB8, 0x00, 0x10, 0x00, 0xA0) };
    constexpr auto inst_addr{ 0 };
    constexpr auto expected_addr{ 0xA0001000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_FALSE));
}

TEST(ee_fwrk_x86_format, format_with_imm32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xB8, 0x00, 0x10, 0x00, 0xA0) };
    constexpr auto inst_addr{ 0 };
    constexpr auto expected_addr{ 0xA0001000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_FALSE));
}

TEST(ee_fwrk_x86_format, format_with_imm64_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x48, 0xB8, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x8F) };
    constexpr auto inst_addr{ 0 };
    constexpr auto expected_addr{ 0x8F00000010000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_FALSE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel8_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0x7E) };
    constexpr auto inst_addr{ 0x8000 };
    constexpr auto expected_addr{ 0x8080 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel8_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0xC0) };
    constexpr auto inst_addr{ 0x803E };
    constexpr auto expected_addr{ 0x8000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel8_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0x7E) };
    constexpr auto inst_addr{ 0x8000008000 };
    constexpr auto expected_addr{ 0x8000008080 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel8_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0xC0) };
    constexpr auto inst_addr{ 0x800000803E };
    constexpr auto expected_addr{ 0x8000008000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel8_16bit_operand_size_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x66, 0xEB, 0x7D) };
    constexpr auto inst_addr{ 0x808000 };
    constexpr auto expected_addr{ 0x8080 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel8_16bit_operand_size_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x66, 0xEB, 0xC0) };
    constexpr auto inst_addr{ 0x80803D };
    constexpr auto expected_addr{ 0x8000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel16_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x66, 0xE9, 0xFC, 0x3F) };
    constexpr auto inst_addr{ 0x800000 };
    constexpr auto expected_addr{ 0x4000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel16_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x66, 0xE9, 0x00, 0xC0) };
    constexpr auto inst_addr{ 0x807FFC };
    constexpr auto expected_addr{ 0x4000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel32_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0xFB, 0xFF, 0xFF, 0x1F) };
    constexpr auto inst_addr{ 0x80000000 };
    constexpr auto expected_addr{ 0xA0000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel32_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0x00, 0x00, 0x00, 0xB0) };
    constexpr auto inst_addr{ 0x9FFFFFFB };
    constexpr auto expected_addr{ 0x50000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel32_wraparound_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0xFB, 0xFF, 0xFF, 0x2F) };
    constexpr auto inst_addr{ 0xE0000000 };
    constexpr auto expected_addr{ 0x10000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel32_wraparound_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0x00, 0x00, 0x00, 0xE0) };
    constexpr auto inst_addr{ 0x0FFFFFFB };
    constexpr auto expected_addr{ 0xF0000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0xFB, 0xFF, 0xFF, 0x1F) };
    constexpr auto inst_addr{ 0x8080000000 };
    constexpr auto expected_addr{ 0x80A0000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0x00, 0x00, 0x00, 0xB0) };
    constexpr auto inst_addr{ 0x809FFFFFFB };
    constexpr auto expected_addr{ 0x8050000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x05, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x3A000000 };
    constexpr auto expected_addr{ 0x7FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x05, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x10000000 };
    constexpr auto expected_addr{ 0x8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_rip_relative_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x05, 0xFA, 0xFF, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x8F80000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_rip_relative_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x05, 0x00, 0x00, 0x00, 0xB0) };
    constexpr auto inst_addr{ 0x8F9FFFFFFA };
    constexpr auto expected_addr{ 0x8F50000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_rip_relative_and_imm8_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x83, 0x3D, 0xF9, 0xFF, 0xFF, 0x7F, 0x42) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x8F80000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_rip_relative_and_imm8_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x83, 0x3D, 0x00, 0x00, 0x00, 0xB0, 0x42) };
    constexpr auto inst_addr{ 0x8F9FFFFFF9 };
    constexpr auto expected_addr{ 0x8F50000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_rip_relative_and_imm32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xC7, 0x05, 0xF6, 0xFF, 0xFF, 0x7F, 0x78, 0x56, 0x34, 0x12) };
    constexpr auto inst_addr{ 0x8F00000000 };

    ASSERT_TRUE(expect_computed_addresses(EE_X86_MODE_64, inst, inst_addr, std::vector{ symbolized_address_info(true, 0x8F80000000), symbolized_address_info(false, 0x12345678) }));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_rip_relative_and_imm32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xC7, 0x05, 0x00, 0x00, 0x00, 0xB0, 0x78, 0x56, 0x34, 0x12) };
    constexpr auto inst_addr{ 0x8F9FFFFFF6 };

    ASSERT_TRUE(expect_computed_addresses(EE_X86_MODE_64, inst, inst_addr, std::vector{ symbolized_address_info(true, 0x8F50000000), symbolized_address_info(false, 0x12345678) }));
}

TEST(ee_fwrk_x86_format, format_with_sib_positive_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x04, 0x25, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x3A000000 };
    constexpr auto expected_addr{ 0x7FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_sib_negative_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x04, 0x25, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x10000000 };
    constexpr auto expected_addr{ 0x8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_sib_positive_absolute_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x04, 0x25, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x7FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_sib_negative_absolute_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x04, 0x25, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0xFFFFFFFF8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_moffs32_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x3A000000 };
    constexpr auto expected_addr{ 0x7FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_moffs32_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x10000000 };
    constexpr auto expected_addr{ 0x8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_moffs64_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x7FFF000000000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_moffs64_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x8FFF000000000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_eip_relative_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0x8B, 0x05, 0xF9, 0xFF, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x80000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_eip_relative_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0x8B, 0x05, 0x00, 0x00, 0x00, 0xB0) };
    constexpr auto inst_addr{ 0x8F9FFFFFF9 };
    constexpr auto expected_addr{ 0x50000000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_sib_negative_32bit_absolute_address_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0x8B, 0x04, 0x25, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_moffs32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0xA1, 0x00, 0x00, 0xFF, 0x7F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x7FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_moffs32_symbolization_64) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0xA1, 0x00, 0x00, 0xFF, 0x8F) };
    constexpr auto inst_addr{ 0x8F00000000 };
    constexpr auto expected_addr{ 0x8FFF0000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_64, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_16bit_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0x8B, 0x06, 0x00, 0x7F) };
    constexpr auto inst_addr{ 0x3A000000 };
    constexpr auto expected_addr{ 0x7F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_16bit_absolute_address_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0x8B, 0x06, 0x00, 0x8F) };
    constexpr auto inst_addr{ 0x10000000 };
    constexpr auto expected_addr{ 0x8F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_moffs16_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0xA1, 0x00, 0x7F) };
    constexpr auto inst_addr{ 0x3A000000 };
    constexpr auto expected_addr{ 0x7F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_moffs16_symbolization_32) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x67, 0xA1, 0x00, 0x8F) };
    constexpr auto inst_addr{ 0x10000000 };
    constexpr auto expected_addr{ 0x8F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_32, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel8_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0x7E) };
    constexpr auto inst_addr{ 0x8000 };
    constexpr auto expected_addr{ 0x8080 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel8_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xEB, 0xC0) };
    constexpr auto inst_addr{ 0x803E };
    constexpr auto expected_addr{ 0x8000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_rel16_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0xFD, 0x3F) };
    constexpr auto inst_addr{ 0x4000 };
    constexpr auto expected_addr{ 0x8000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_rel16_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xE9, 0x00, 0xC0) };
    constexpr auto inst_addr{ 0x7FFD };
    constexpr auto expected_addr{ 0x4000 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_positive_absolute_address_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x06, 0x00, 0x7F) };
    constexpr auto inst_addr{ 0x1000 };
    constexpr auto expected_addr{ 0x7F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_modrm_negative_absolute_address_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0x8B, 0x06, 0x00, 0x8F) };
    constexpr auto inst_addr{ 0x1000 };
    constexpr auto expected_addr{ 0x8F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_positive_moffs16_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x7F) };
    constexpr auto inst_addr{ 0x1000 };
    constexpr auto expected_addr{ 0x7F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}

TEST(ee_fwrk_x86_format, format_with_negative_moffs16_symbolization_16) {

    constexpr static auto inst{ make_array<ee_byte_t>(0xA1, 0x00, 0x8F) };
    constexpr auto inst_addr{ 0x1000 };
    constexpr auto expected_addr{ 0x8F00 };

    ASSERT_TRUE(expect_computed_address(EE_X86_MODE_16, inst, inst_addr, expected_addr, EE_TRUE));
}