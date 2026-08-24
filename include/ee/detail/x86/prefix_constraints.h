#ifndef EE_X86_PREFIX_CONSTRAINTS_IG
#define EE_X86_PREFIX_CONSTRAINTS_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

typedef enum {

    EE_PRV_X86_PREFAM_NIL,
    EE_PRV_X86_PREFAM_GRP,
    EE_PRV_X86_PREFAM_GRP_REX,
    EE_PRV_X86_PREFAM_VEX,
    EE_PRV_X86_PREFAM_ADDRESS_SIZE_OVERRIDE

} ee_prv_x86_prefix_family_t;

typedef struct {

    ee_prv_x86_prefix_family_t family;

} ee_prv_x86_prefix_constraints_t;

typedef struct {

    ee_prv_x86_prefix_constraints_t hdr;
    ee_int16_t mandatory_prefix;
    ee_int16_t prohibited_prefixes[3];

} ee_prv_x86_grp_prefix_constraints_t;

typedef struct {

    ee_prv_x86_grp_prefix_constraints_t grp_cons;
    ee_int16_t mandatory_rex_bits;
    ee_int16_t prohibited_rex_bits;

} ee_prv_x86_grp_rex_prefix_constraints_t;

typedef struct {

    ee_prv_x86_prefix_constraints_t hdr;
    ee_int16_t mandatory_l;
    ee_int16_t mandatory_pp;
    ee_int16_t mandatory_mmmmm;
    ee_int16_t mandatory_w_x86;
    ee_int16_t mandatory_w_x64;
    ee_int16_t mandatory_raw_vvvv;

} ee_prv_x86_vex_prefix_constraints_t;

typedef struct {

    ee_prv_x86_prefix_constraints_t hdr;
    ee_bool_t mandatory_for_x86;
    ee_bool_t mandatory_for_x64;
    ee_bool_t prohibited_for_x86;
    ee_bool_t prohibited_for_x64;

} ee_prv_x86_address_size_override_prefix_constraints_t;

#endif
