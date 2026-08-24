#ifndef EE_X86_OPERAND_CONSTRAINTS_IG
#define EE_X86_OPERAND_CONSTRAINTS_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

typedef enum {

    EE_PRV_X86_OPDFAM_NULL,
    EE_PRV_X86_OPDFAM_MODRM,
    EE_PRV_X86_OPDFAM_MODRM_BITS,
    EE_PRV_X86_OPDFAM_BYTE_RANGE

} ee_prv_x86_operand_family_t;

typedef struct {

    ee_prv_x86_operand_family_t family;

} ee_prv_x86_operand_constraints_t;

typedef struct {

    ee_prv_x86_operand_constraints_t hdr;
    ee_int16_t mandatory_mod;
    ee_int16_t mandatory_rm;
    ee_int16_t mandatory_reg;
    ee_int16_t prohibited_mod;
    ee_int16_t prohibited_rm;
    ee_int16_t prohibited_reg;

} ee_prv_x86_modrm_operand_constraints_t;

typedef struct {

    ee_prv_x86_operand_constraints_t hdr;
    ee_int16_t mandatory_mod_bit_comb;
    ee_int16_t mandatory_rm_bit_comb;
    ee_int16_t mandatory_reg_bit_comb;
    ee_int16_t prohibited_mod_bit_comb;
    ee_int16_t prohibited_rm_bit_comb;
    ee_int16_t prohibited_reg_bit_comb;

} ee_prv_x86_modrm_operand_bit_constraints_t;

typedef struct {

    ee_prv_x86_operand_constraints_t hdr;
    ee_byte_t first;
    ee_byte_t last;

} ee_prv_x86_byte_range_operand_constraints_t;

#endif
