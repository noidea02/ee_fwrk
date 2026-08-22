#ifndef EE_X86_OPERAND_CONSTRAINTS_IMPL_IG
#define EE_X86_OPERAND_CONSTRAINTS_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/constants.h"
#include "ee/detail/x86/operand_constraints.h"

/* ModR/M operand constraints
*/

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_ANY_M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory mod */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory rm */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory reg*/
    3,                           /* prohibited mod */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* prohibited rm */
    EE_PRV_X86_CONSTRAINT_EMPTY  /* prohibited reg */
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_ANY_R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_MOD0 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG1R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG2R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    2,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG3R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG4R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    4,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG5R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    5,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG6R = {
    { EE_PRV_X86_OPDFAM_MODRM },
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    6,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7 = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    7,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7M = {
    { EE_PRV_X86_OPDFAM_MODRM },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    7,
    3,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

const ee_prv_x86_modrm_operand_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG7R = {
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

const ee_prv_x86_modrm_operand_bit_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0XX_M = {
    { EE_PRV_X86_OPDFAM_MODRM_BITS },
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory mod bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory rm bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* mandatory reg bit combination */
    3,                           /* prohibited mod bit combination */
    EE_PRV_X86_CONSTRAINT_EMPTY, /* prohibited rm bit combination */
    4                            /* prohibited reg bit combination */
};

const ee_prv_x86_modrm_operand_bit_constraints_t EE_PRV_X86_OPRDCONS_MODRM_REG0XX_RM0XX_R = {
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

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_0A = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0x0a
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc1
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc2
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc3
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc4
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc5
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc8
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc9
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcc
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcd
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xce
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_CF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xcf
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd0
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd1
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd4
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd5
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D6 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd6
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd8
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd9
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xda
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdb
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdc
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdd
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xde
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_DF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xdf
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe0
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe1
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe2
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe3
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe4
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe5
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe8
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe9
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xea
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xeb
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xec
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_ED = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xed
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xee
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_EF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xef
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F0 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf0
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F1 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf1
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F2 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf2
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F3 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf3
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F4 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf4
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F5 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf5
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F6 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf6
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F8 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf8
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F9 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf9
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FA = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfa
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FB = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfb
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FC = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfc
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FD = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfd
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FE = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xfe
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_FF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xff
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C0tC7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc0,
    0xc7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_C8tCF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xc8,
    0xcf
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D0tD7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd0,
    0xd7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_D8tDF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xd8,
    0xdf
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E0tE7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe0,
    0xe7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_E8tEF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xe8,
    0xef
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F0tF7 = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf0,
    0xf7
};

const ee_prv_x86_byte_range_operand_constraints_t EE_PRV_X86_OPRDCONS_BYTE_RANGE_F8tFF = {
    { EE_PRV_X86_OPDFAM_BYTE_RANGE },
    0xf8,
    0xff
};

#endif
