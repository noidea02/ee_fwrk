#include <array>

#include <gtest/gtest.h>

#include <ee/ee_x86.h>

#include "test_x86_disasm_utils.hpp"

using namespace ee_test;

namespace {

    constexpr std::array<ee_byte_t, 2> INC_EAX{ 0xff, 0xc0 }; // INC EAX
    constexpr std::array<ee_byte_t, 3> OSO_INC_EAX{ 0x66, 0xff, 0xc0 }; // INC EAX
    constexpr std::array<ee_byte_t, 3> ASO_INC_EAX{ 0x67, 0xff, 0xc0 }; // INC EAX
    constexpr std::array<ee_byte_t, 4> OSO_ASO_INC_EAX{ 0x66, 0x67, 0xff, 0xc0 }; // INC EAX
    constexpr std::array<ee_byte_t, 4> ASO_OSO_INC_EAX{ 0x67, 0x66, 0xff, 0xc0 }; // INC EAX
    constexpr std::array<ee_byte_t, 3> REXW_INC_RAX{ 0x48, 0xff, 0xc0 }; // INC RAX
    constexpr std::array<ee_byte_t, 4> OSO_REXW_INC_RAX{ 0x66, 0x48, 0xff, 0xc0 }; // INC RAX
    constexpr std::array<ee_byte_t, 4> REXW_OSO_INC_EAX{ 0x48, 0x66, 0xff, 0xc0 }; // INC EAX
}

TEST(ee_fwrk_x86, disasm_16_inc_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, INC_EAX.data(), INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_inc_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_INC_EAX.data(), OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_inc_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_INC_EAX.data(), ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_inc_66h_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, OSO_ASO_INC_EAX.data(), OSO_ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_16_inc_67h_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_16, ASO_OSO_INC_EAX.data(), ASO_OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_inc_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, INC_EAX.data(), INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_inc_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_INC_EAX.data(), OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_inc_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_INC_EAX.data(), ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_inc_66h_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, OSO_ASO_INC_EAX.data(), OSO_ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_32_inc_67h_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_32, ASO_OSO_INC_EAX.data(), ASO_OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(2, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, INC_EAX.data(), INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_INC_EAX.data(), OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_EAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_INC_EAX.data(), ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_66h_67h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_ASO_INC_EAX.data(), OSO_ASO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_67h_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_TRUE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, ASO_OSO_INC_EAX.data(), ASO_OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_48h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(3, { EE_FALSE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_INC_RAX.data(), REXW_INC_RAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_66h_48h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_RAX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, OSO_REXW_INC_RAX.data(), OSO_REXW_INC_RAX.size(), &is));
    EXPECT_EQ(expected, is);
}

TEST(ee_fwrk_x86, disasm_64_inc_48h_66h_FFh) {

    static constexpr auto expected{ create_x86_disasm_output_with_one_operand(4, { EE_TRUE, EE_FALSE }, EE_X86_INSTRUCTION_INC, EE_X86_GPP_REGISTER_AX) };

    ee_x86_disasm_output is{};
    EXPECT_TRUE(ee_x86_disasm(EE_X86_MODE_64, REXW_OSO_INC_EAX.data(), REXW_OSO_INC_EAX.size(), &is));
    EXPECT_EQ(expected, is);
}