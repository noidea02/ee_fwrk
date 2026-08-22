#include <array>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    constexpr std::array<ee_byte_t, 5> M66_ADCX{ 0x66, 0x0F, 0x38, 0xF6, 0x00 }; // adcx eax, dword ptr [eax]
    constexpr std::array<ee_byte_t, 6> M66_ASO_ADCX{ 0x66, 0x67, 0x0F, 0x38, 0xF6, 0x00 }; // adcx eax, dword ptr [bx+si]
    constexpr std::array<ee_byte_t, 6> ASO_M66_ADCX{ 0x67, 0x66, 0x0F, 0x38, 0xF6, 0x00 }; // adcx eax, dword ptr [bx+si]
    constexpr std::array<ee_byte_t, 6> REXW_M66_ADCX{ 0x48, 0x66, 0x0F, 0x38, 0xF6, 0x00 }; // adcx eax, qword ptr [eax]
    constexpr std::array<ee_byte_t, 6> M66_REXW_ADCX{ 0x66, 0x48, 0x0F, 0x38, 0xF6, 0x00 }; // adcx rax, qword ptr [rax]
}

TEST(ee_fwrk_x86, disasm_16_adcx_66h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, M66_ADCX.data(), M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_adcx_66h_67h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, M66_ASO_ADCX.data(), M66_ASO_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_adcx_67h_68h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_M66_ADCX.data(), ASO_M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_adcx_66h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_EAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, M66_ADCX.data(), M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_adcx_66h_67h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, M66_ASO_ADCX.data(), M66_ASO_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_adcx_67h_68h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_BX, EE_X86_GPP_REGISTER_SI }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_M66_ADCX.data(), ASO_M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_adcx_66h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(5, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, M66_ADCX.data(), M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_adcx_66h_48h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_RAX, { EE_X86_POINTER_QWORD, EE_X86_GPP_REGISTER_RAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, M66_REXW_ADCX.data(), M66_REXW_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_adcx_48h_68h_0Fh_38h_F6h_00h) {

    static constexpr auto expected{ create_x86_disasm_output_with_two_operands(6, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_ADCX,
        EE_X86_GPP_REGISTER_EAX, { EE_X86_POINTER_DWORD, EE_X86_GPP_REGISTER_RAX }) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_M66_ADCX.data(), REXW_M66_ADCX.size(), &is));
    EXPECT_EQ(expected, is);
}