#ifndef EE_X86_MODRM_TABLES_IG
#define EE_X86_MODRM_TABLES_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_x86.h"

/* Types
*/

typedef enum {

    EE_PRV_X86_EFFADDR_NONE,
    EE_PRV_X86_EFFADDR_PTR,
    EE_PRV_X86_EFFADDR_REG

} ee_prv_x86_modrm_effective_address_type_t;

typedef struct {

    /* x64 extended general-purpose registers (R8x-R15x) as well as SSE registers get derived using the index field.
    */

    ee_size_t index;
    ee_x86_gpp_register_type_t gpp8;
    ee_x86_gpp_register_type_t gpp16;
    ee_x86_gpp_register_type_t gpp32;
    ee_x86_gpp_register_type_t gpp64;

} ee_prv_x86_modrm_reg_pack_t;

typedef struct {

    /* If both reg0 and reg1 are NOT 0, [reg0+reg1] is implied. If both are 0, SIB is implied. For modrm32, reg1 is always 0.
    */

    const ee_prv_x86_modrm_reg_pack_t* reg0;
    const ee_prv_x86_modrm_reg_pack_t* reg1;

} ee_prv_x86_modrm_reg_pair_t;

typedef struct {

    /* If "base" is 0, either [disp16] (if in modrm16 mode) or [disp32] (if in modrm32 mode) is implied.
    */

    const ee_prv_x86_modrm_reg_pair_t* base;
    ee_int32_t disp_bits;

} ee_prv_x86_modrm_ptr_t;

typedef struct {

    ee_prv_x86_modrm_effective_address_type_t type;
    ee_prv_x86_modrm_ptr_t ptr;
    const ee_prv_x86_modrm_reg_pack_t* reg;

} ee_prv_x86_modrm_effective_address_t;

typedef struct {

    ee_prv_x86_modrm_effective_address_t eff_addr;
    const ee_prv_x86_modrm_reg_pack_t* reg;

} ee_prv_x86_modrm_mapping_t;

typedef struct {

    /* If "base" is 0, either baseless disp32 (for MOD 00B) or [RBP] plus disp8 (MOD 01B) / disp32 (MOD 10B) is implied.
       If "scaled_index" is 0, no scaled index is available.
    */

    const ee_prv_x86_modrm_reg_pack_t* base;
    const ee_prv_x86_modrm_reg_pack_t* scaled_index;
    ee_uint8_t scaling;

} ee_prv_x86_modrm32_sib_mapping_t;

typedef struct {

    /* If "is_pointer" is false, only "base_reg" is valid, which in turn denotes a raw (non-ponter) register operand.
    */

    ee_bool_t is_pointer;
    ee_bool_t uses_rip_relative_addressing;
    const ee_prv_x86_modrm_reg_pack_t* base_reg;
    const ee_prv_x86_modrm_reg_pack_t* index_reg;
    ee_uint8_t scaling_factor;
    ee_uint32_t disp_bits;
    ee_int64_t disp;

} ee_prv_x86_modrm_resolved_effective_address_t;

/* Fields
*/

extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_EAX;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_ECX;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_EDX;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_EBX;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_ESP;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_EBP;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_ESI;
extern const ee_prv_x86_modrm_reg_pack_t EE_PRV_X86_MODRM_REG_PACK_EDI;

extern const ee_prv_x86_modrm_reg_pack_t* EE_PRV_X86_MODRM_REG_PACKS[];
extern const ee_size_t EE_PRV_X86_MODRM_REG_PACKS_COUNT;

extern const ee_prv_x86_modrm_mapping_t EE_PRV_X86_MODRM16_TABLE[];
extern const ee_size_t EE_PRV_X86_MODRM16_TABLE_COUNT;

extern const ee_prv_x86_modrm_mapping_t EE_PRV_X86_MODRM32_TABLE[];
extern const ee_size_t EE_PRV_X86_MODRM32_TABLE_COUNT;

extern const ee_prv_x86_modrm32_sib_mapping_t EE_PRV_X86_MODRM32_SIB_TABLE[];
extern const ee_size_t EE_PRV_X86_MODRM32_SIB_TABLE_COUNT;

extern const ee_prv_x86_modrm32_sib_mapping_t EE_PRV_X86_MODRM32_VSIB_TABLE[];
extern const ee_size_t EE_PRV_X86_MODRM32_VSIB_TABLE_COUNT;

#endif
