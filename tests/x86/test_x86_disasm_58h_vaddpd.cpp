#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include <ee_support/utils.hpp>

#include "test_x86_disasm_utils.hpp"

using namespace ee_supp;
using namespace ee_test;

namespace {

    constexpr auto VADDPD_XMM0_XMM1_XMM2{ make_array<ee_byte_t>(0xc5, 0xf1, 0x58, 0xc2) };
    constexpr auto VADDPD_XMM0_XMM1_XMM8{ make_array<ee_byte_t>(0xc4, 0xc1, 0x71, 0x58, 0xc0) };
    constexpr auto VADDPD_XMM0_XMM9_XMM2{ make_array<ee_byte_t>(0xc5, 0xb1, 0x58, 0xc2) };
    constexpr auto VADDPD_XMM8_XMM1_XMM2{ make_array<ee_byte_t>(0xc5, 0x71, 0x58, 0xc2) };
    constexpr auto VADDPD_XMM0_XMM1_XMMWORD_PTR_R8{ make_array<ee_byte_t>(0xc4, 0xc1, 0x71, 0x58, 0x00) };
    constexpr auto VADDPD_XMM0_XMM1_XMMWORD_PTR_RCX{ make_array<ee_byte_t>(0xc5, 0xf1, 0x58, 0x01) };
    constexpr auto VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX{ make_array<ee_byte_t>(0xc5, 0xf5, 0x58, 0x00) };
}

TEST(ee_fwrk_x86, disasm_32_vaddpd_C5h_F1h_58h_C2h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, VADDPD_XMM0_XMM1_XMM2.data(), VADDPD_XMM0_XMM1_XMM2.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C4h_C1h_71h_58h_C0h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 8 }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_XMM0_XMM1_XMM8.data(), VADDPD_XMM0_XMM1_XMM8.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C5h_B1h_58h_C2h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 9 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_XMM0_XMM9_XMM2.data(), VADDPD_XMM0_XMM9_XMM2.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C5h_71h_58h_C2h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 8 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, { EE_X86_REGISTER_AVX_128, 128, 2 }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_XMM8_XMM1_XMM2.data(), VADDPD_XMM8_XMM1_XMM2.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C4h_C1h_71h_58h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd_with_extended_base{ EE_X86_POINTER_XMMWORD, 8, 64 }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_XMM0_XMM1_XMMWORD_PTR_R8.data(), VADDPD_XMM0_XMM1_XMMWORD_PTR_R8.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C5h_F1h_58h_01h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_128, 128, 0 },
        { EE_X86_REGISTER_AVX_128, 128, 1 }, x86_ptr_opd{ EE_X86_POINTER_XMMWORD, EE_X86_GPP_REGISTER_RCX }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_XMM0_XMM1_XMMWORD_PTR_RCX.data(), VADDPD_XMM0_XMM1_XMMWORD_PTR_RCX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_16_vaddpd_C5h_F5h_58h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_256, 256, 0 },
        { EE_X86_REGISTER_AVX_256, 256, 1 }, x86_ptr_opd{ EE_X86_POINTER_YMMWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.data(), VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_32_vaddpd_C5h_F5h_58h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_256, 256, 0 },
        { EE_X86_REGISTER_AVX_256, 256, 1 }, x86_ptr_opd{ EE_X86_POINTER_YMMWORD, EE_X86_GPP_REGISTER_EAX }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.data(), VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}

TEST(ee_fwrk_x86, disasm_64_vaddpd_C5h_F5h_58h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_three_operands(4, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_VADDPD, { EE_X86_REGISTER_AVX_256, 256, 0 },
        { EE_X86_REGISTER_AVX_256, 256, 1 }, x86_ptr_opd{ EE_X86_POINTER_YMMWORD, EE_X86_GPP_REGISTER_RAX }) };

    ee_x86_disasm_output out{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.data(), VADDPD_YMM0_YMM1_YMMWORD_PTR_EAX.size(), &out));
    EXPECT_EQ(expected, out);
}