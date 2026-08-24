#ifndef EE_X86_OPERAND_CONSTRAINTS_IMPL_IG
#define EE_X86_OPERAND_CONSTRAINTS_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/constants.h"
#include "ee/detail/x86/operand_constraints.h"

/* ModR/M operand constraints
*/

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_ANY_M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory mod */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory rm */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory reg*/
    3,                           /* prohibited mod */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* prohibited rm */
    EE_PRV_X86_CONSTRAINT_EMPTY  /* prohibited reg */
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_ANY_R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_MOD0 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    7,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    7,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    7,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

/* ModR/M operand bit constraints
*/

static const ee_prv_x86_modrm_operand_bit_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0XX_M = {
    { EE_PRV_X86_OPDFAM_MODRM_BITS },
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory mod bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory rm bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory reg bit combination */
    3,                           /* prohibited mod bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* prohibited rm bit combination */
    4                            /* prohibited reg bit combination */
};

static const ee_prv_x86_modrm_operand_bit_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0XX_RM0XX_R = {
    { EE_PRV_X86_OPDFAM_MODRM_BITS },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    4
};

/* Byte range operand constraints
*/

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_0A = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0x0a
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc1
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc2
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc3
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc4
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc5
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc8
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc9
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcc
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcd
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xce
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcf
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd0
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd1
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd4
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd5
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D6 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd6
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd8
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd9
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xda
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdb
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdc
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdd
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xde
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdf
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe0
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe1
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe2
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe3
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe4
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe5
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe8
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe9
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xea
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xeb
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xec
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_ED = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xed
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xee
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xef
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf0
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf1
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf2
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf3
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf4
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf5
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F6 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf6
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf8
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf9
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfa
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfb
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfc
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfd
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfe
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xff
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C0tC7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc0,
    0xc7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C8tCF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc8,
    0xcf
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D0tD7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd0,
    0xd7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D8tDF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd8,
    0xdf
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E0tE7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe0,
    0xe7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E8tEF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe8,
    0xef
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F0tF7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf0,
    0xf7
};

static const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F8tFF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf8,
    0xff
};

#endif
