#include <string>
#include <string_view>

#include <gtest/gtest.h>

#include <ee/ee_x86_format.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    void expect_disasm_fail(ee_x86_mode_t mode, std::span<const ee_byte_t> inst) {

        ee_x86_disasm_output dis_out{};
        ASSERT_FALSE(ee_x86_disasm(mode, inst.data(), inst.size(), &dis_out));
    }

    void expect_format(ee_x86_mode_t mode, std::span<const ee_byte_t> inst, ee_uint64_t addr, std::string_view expected) {

        ee_x86_disasm_output dis_out{};
        ASSERT_TRUE(ee_x86_disasm(mode, inst.data(), inst.size(), &dis_out));

        ee_size_t req_size{ 256 };
        const auto out{ std::make_unique<ee_ascii_char_t[]>(req_size) };

        ASSERT_TRUE(ee_x86_format(mode, addr, &dis_out, out.get(), &req_size));
        EXPECT_STREQ(out.get(), std::string(expected).c_str());
    }
}

/* 32-bit regression tests
*/

TEST(ee_fwrk_x86_format_regression, mode32_C4h_C2h_41h_90h_04h_20h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xC2, 0x41, 0x90, 0x04, 0x20) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[eax+xmm4], xmm7");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_C2h_41h_90h_04h_61h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xC2, 0x41, 0x90, 0x04, 0x61) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[ecx+xmm4*2], xmm7");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E2h_60h_F2h_01h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x60, 0xF2, 0x01) };
    constexpr std::string_view expected("andn eax, ebx, dword ptr[ecx]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E2h_78h_F2h_33h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x78, 0xF2, 0x33) };
    constexpr std::string_view expected("andn esi, eax, dword ptr[ebx]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E2h_79h_90h_04h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x79, 0x90, 0x04, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E2h_7Dh_19h_E6h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x7D, 0x19, 0xE6) };
    constexpr std::string_view expected("vbroadcastsd ymm4, xmm6");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E2h_69h_90h_04h_08h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x69, 0x90, 0x04, 0x08) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[eax+xmm1], xmm2");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_04h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x04, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_05h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x05, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_08h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x08, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_09h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x09, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_60h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x60, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_61h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x61, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_62h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x62, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_71h_63h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x71, 0x63, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_04h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x04, 0x00, 0x00) };
    constexpr std::string_view expected("vpermilps xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_05h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x05, 0x00, 0x00) };
    constexpr std::string_view expected("vpermilpd xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_08h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x08, 0x00, 0x00) };
    constexpr std::string_view expected("vroundps xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_09h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x09, 0x00, 0x00) };
    constexpr std::string_view expected("vroundpd xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_60h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x60, 0x00, 0x00) };
    constexpr std::string_view expected("vpcmpestrm xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_61h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x61, 0x00, 0x00) };
    constexpr std::string_view expected("vpcmpestri xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_62h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x62, 0x00, 0x00) };
    constexpr std::string_view expected("vpcmpistrm xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C4h_E3h_79h_63h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE3, 0x79, 0x63, 0x00, 0x00) };
    constexpr std::string_view expected("vpcmpistri xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F0h_28h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF0, 0x28, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F1h_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF1, 0x70, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F2h_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF2, 0x70, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F3h_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF3, 0x70, 0x00, 0x00) };
    expect_disasm_fail(EE_X86_MODE_32, in);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F8h_28h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF8, 0x28, 0x00) };
    constexpr std::string_view expected("vmovaps xmm0, xmmword ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_F9h_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF9, 0x70, 0x00, 0x00) };
    constexpr std::string_view expected("vpshufd xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_FAh_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFA, 0x70, 0x00, 0x00) };
    constexpr std::string_view expected("vpshufhw xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_C5h_FBh_70h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFB, 0x70, 0x00, 0x00) };
    constexpr std::string_view expected("vpshuflw xmm0, xmmword ptr[eax], 00h");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F2h_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf2, 0x00, 0x00) };
    constexpr std::string_view expected("xacquire lock add byte ptr[eax], al");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F2h_0Fh_10h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf2, 0x0f, 0x10, 0x00) };
    constexpr std::string_view expected("lock movsd xmm0, qword ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F2h_0Fh_11h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf2, 0x0f, 0x11, 0x00) };
    constexpr std::string_view expected("lock movsd qword ptr[eax], xmm0");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F2h_0Fh_12h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf2, 0x0f, 0x12, 0x00) };
    constexpr std::string_view expected("lock movddup xmm0, qword ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F3h_0Fh_10h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf3, 0x0f, 0x10, 0x00) };
    constexpr std::string_view expected("lock movss xmm0, dword ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F3h_0Fh_11h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf3, 0x0f, 0x11, 0x00) };
    constexpr std::string_view expected("lock movss dword ptr[eax], xmm0");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F3h_0Fh_12h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf3, 0x0f, 0x12, 0x00) };
    constexpr std::string_view expected("lock movsldup xmm0, xmmword ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F0h_F3h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf0, 0xf3, 0x90) };
    constexpr std::string_view expected("lock pause");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode32_F2h_86h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf2, 0x86, 0x00) };
    constexpr std::string_view expected("xacquire xchg al, byte ptr[eax]");

    expect_format(EE_X86_MODE_32, in, 0, expected);
}

/* 64-bit regression tests
*/

TEST(ee_fwrk_x86_format_regression, mode64_0Fh_50h_C0h) {

    constexpr auto in{ make_array<ee_byte_t>(0x0F, 0x50, 0xC0) };
    constexpr std::string_view expected("movmskps eax, xmm0");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_41h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0x41, 0x90) };
    constexpr std::string_view expected("xchg eax, r8d");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_41h_F3h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0x41, 0xf3, 0x90) };
    constexpr std::string_view expected("pause");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_48h_0Fh_07h) {

    constexpr auto in{ make_array<ee_byte_t>(0x48, 0x0F, 0x07) };
    constexpr std::string_view expected("sysretq");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_48h_CAh_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0x48, 0xCA, 0x00, 0x00) };
    constexpr std::string_view expected("retfq 00h");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_49h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0x49, 0x90) };
    constexpr std::string_view expected("xchg rax, r8");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_66h_0Fh_50h_C0h) {

    constexpr auto in{ make_array<ee_byte_t>(0x66, 0x0F, 0x50, 0xC0) };
    constexpr std::string_view expected("movmskpd eax, xmm0");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_67h_C4h_E2h_60h_F2h_01h) {

    constexpr auto in{ make_array<ee_byte_t>(0x67, 0xC4, 0xE2, 0x60, 0xF2, 0x01) };
    constexpr std::string_view expected("andn eax, ebx, dword ptr[ecx]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_02h_69h_90h_04h_88h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x02, 0x69, 0x90, 0x04, 0x88) };
    constexpr std::string_view expected("vpgatherdd xmm8, dword ptr[r8+xmm9*4], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_02h_6Dh_90h_7Ch_1Dh_12h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x02, 0x6D, 0x90, 0x7C, 0x1D, 0x12) };
    constexpr std::string_view expected("vpgatherdd ymm15, dword ptr[r13+ymm11+12h], ymm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_02h_6Dh_90h_04h_1Dh_44h_33h_22h_11h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x02, 0x6D, 0x90, 0x04, 0x1D, 0x44, 0x33, 0x22, 0x11) };
    constexpr std::string_view expected("vpgatherdd ymm8, dword ptr[ymm11+11223344h], ymm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_02h_6Dh_90h_04h_C8h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x02, 0x6D, 0x90, 0x04, 0xC8) };
    constexpr std::string_view expected("vpgatherdd ymm8, dword ptr[r8+ymm9*8], ymm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_42h_B0h_F2h_02h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x42, 0xB0, 0xF2, 0x02) };
    constexpr std::string_view expected("andn r8, r9, qword ptr[r10]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_62h_69h_90h_04h_48h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0x62, 0x69, 0x90, 0x04, 0x48) };
    constexpr std::string_view expected("vpgatherdd xmm8, dword ptr[rax+xmm1*2], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_A2h_69h_90h_04h_08h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xA2, 0x69, 0x90, 0x04, 0x08) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[rax+xmm9], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_C2h_69h_90h_04h_08h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xC2, 0x69, 0x90, 0x04, 0x08) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[r8+xmm1], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_38h_F2h_84h_41h_44h_33h_22h_11h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x38, 0xF2, 0x84, 0x41, 0x44, 0x33, 0x22, 0x11) };
    constexpr std::string_view expected("andn eax, r8d, dword ptr[rcx+rax*2+11223344h]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_69h_90h_44h_1Dh_21h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x69, 0x90, 0x44, 0x1D, 0x21) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[rbp+xmm3+21h], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_69h_90h_84h_1Dh_44h_33h_22h_11h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x69, 0x90, 0x84, 0x1D, 0x44, 0x33, 0x22, 0x11) };
    constexpr std::string_view expected("vpgatherdd xmm0, dword ptr[rbp+xmm3+11223344h], xmm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_78h_F2h_33h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x78, 0xF2, 0x33) };
    constexpr std::string_view expected("andn esi, eax, dword ptr[rbx]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_79h_18h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x79, 0x18, 0x00) };
    constexpr std::string_view expected("vbroadcastss xmm0, dword ptr[rax]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_79h_90h_04h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x79, 0x90, 0x04, 0x00) };
    expect_disasm_fail(EE_X86_MODE_64, in);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_7Dh_18h_FFh) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0x7D, 0x18, 0xFF) };
    constexpr std::string_view expected("vbroadcastss ymm7, xmm7");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C4h_E2h_E0h_F2h_01h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC4, 0xE2, 0xE0, 0xF2, 0x01) };
    constexpr std::string_view expected("andn rax, rbx, qword ptr[rcx]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_F8h_50h_C0h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF8, 0x50, 0xC0) };
    constexpr std::string_view expected("vmovmskps eax, xmm0");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_F9h_5Ah_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF9, 0x5A, 0x00) };
    constexpr std::string_view expected("vcvtpd2ps xmm0, xmmword ptr[rax]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_F9h_5Ah_FFh) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF9, 0x5A, 0xFF) };
    constexpr std::string_view expected("vcvtpd2ps xmm7, xmm7");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_F9h_E6h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF9, 0xE6, 0x00) };
    constexpr std::string_view expected("vcvttpd2dq xmm0, xmmword ptr[rax]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_F9h_E6h_FFh) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xF9, 0xE6, 0xFF) };
    constexpr std::string_view expected("vcvttpd2dq xmm7, xmm7");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_FCh_50h_D2h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFC, 0x50, 0xD2) };
    constexpr std::string_view expected("vmovmskps edx, ymm2");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_FDh_5Ah_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFD, 0x5A, 0x00) };
    constexpr std::string_view expected("vcvtpd2ps xmm0, ymmword ptr[rax]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_FDh_5Ah_FFh) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFD, 0x5A, 0xFF) };
    constexpr std::string_view expected("vcvtpd2ps xmm7, ymm7");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_FDh_E6h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFD, 0xE6, 0x00) };
    constexpr std::string_view expected("vcvttpd2dq xmm0, ymmword ptr[rax]");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_C5h_FDh_E6h_FFh) {

    constexpr auto in{ make_array<ee_byte_t>(0xC5, 0xFD, 0xE6, 0xFF) };
    constexpr std::string_view expected("vcvttpd2dq xmm7, ymm7");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_CAh_00h_00h) {

    constexpr auto in{ make_array<ee_byte_t>(0xCA, 0x00, 0x00) };
    constexpr std::string_view expected("retf 00h");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_F3h_41h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf3, 0x41, 0x90) };
    constexpr std::string_view expected("pause");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}

TEST(ee_fwrk_x86_format_regression, mode64_F3h_49h_90h) {

    constexpr auto in{ make_array<ee_byte_t>(0xf3, 0x49, 0x90) };
    constexpr std::string_view expected("pause");

    expect_format(EE_X86_MODE_64, in, 0, expected);
}