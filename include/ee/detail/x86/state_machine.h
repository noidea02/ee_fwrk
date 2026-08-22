#ifndef EE_X86_STATE_MACHINE_IG
#define EE_X86_STATE_MACHINE_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_x86.h"

/* Types
*/

typedef struct {

    ee_byte_t grp_prefixes[4]; /* Active group prefixes are stored at index [x-1], with x being the group number. */
    ee_byte_t stacked_grp1_prefix; /* Added to account for HLE and MPX, which may stack group 1 prefixes. */
    ee_byte_t preliminary_mandatory_grp_prefix;
    ee_byte_t confirmed_mandatory_grp_prefix;
    ee_byte_t rex_prefix;
    ee_byte_t vex_core_bytes[2];
    ee_size_t num_vex_core_bytes;

} ee_prv_x86_prefix_state_t;

typedef struct {

    ee_bool_t is_eligible_for_branch_hint;
    ee_bool_t is_eligible_for_pseudonymization;
    ee_uint32_t is_eligible_for_hle; /* Stores the exact HLE flag for the instruction; flag will be greater than 0 if the instruction is eligible for HLE. */
    ee_bool_t is_eligible_for_bnd_prefix;

} ee_prv_x86_instruction_flags_t;

struct ee_prv_x86_state;
typedef ee_bool_t(*ee_prv_x86_process_operand_bytes_t)(struct ee_prv_x86_state*);

typedef struct ee_prv_x86_state {

    ee_bool_t ignored;
    ee_x86_mode_t active_mode;
    const ee_byte_t* bytes;
    ee_size_t num_bytes;
    ee_size_t byte_index;
    ee_prv_x86_prefix_state_t active_prefixes;
    ee_x86_prefixes_t final_prefixes;
    ee_x86_instruction_type_t instruction;
    ee_prv_x86_instruction_flags_t instruction_flags;
    ee_prv_x86_process_operand_bytes_t process_operand_bytes;
    ee_byte_t operand_base_byte;
    ee_x86_operand_t operands[4];
    ee_size_t num_operands;

} ee_prv_x86_state_t;

/* Functions (required by operand processors)
*/

ee_bool_t ee_prv_x86_state_just_advance_bytes(ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_is_16_bit_mode_active(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_is_64_bit_mode_active(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_has_active_operand_size_override(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_has_active_address_size_override(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_has_active_rex_w(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_has_active_vex_prefix(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_is_vex_vvvv_set(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_hints_at_16_bit_operand_usage(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_hints_at_16_bit_address_usage(const ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_consume_immediate_bytes(ee_prv_x86_state_t* distate, ee_size_t num_bits, ee_int64_t* immb);
ee_x86_segment_register_type_t ee_prv_x86_prefix_state_to_segment_register(const ee_prv_x86_prefix_state_t* ps);
void ee_prv_x86_state_get_dynamic_modrm_operand_info(const ee_prv_x86_state_t* distate, ee_x86_pointer_type_t* ptr_type, ee_size_t* reg_size_bits);
ee_bool_t ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(const ee_prv_x86_state_t* distate, ee_uint8_t* vex_reg_spec, ee_x86_pointer_type_t* ptr_type, ee_x86_register_type_t* reg_type, ee_size_t* reg_size_bits);
ee_bool_t ee_prv_x86_state_vex_get_register_specifier(const ee_prv_x86_state_t* distate, ee_uint8_t* reg_spec);
ee_bool_t ee_prv_x86_state_append_segreg_operand(ee_prv_x86_state_t* distate, ee_x86_segment_register_type_t reg);
ee_bool_t ee_prv_x86_state_append_gppreg_operand_as_is(ee_prv_x86_state_t* distate, ee_x86_gpp_register_type_t reg);
ee_bool_t ee_prv_x86_state_append_gppreg_operand_with_prefix_adjustments(ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t* gppreg_pack);
ee_bool_t ee_prv_x86_state_append_numreg_operand(ee_prv_x86_state_t* distate, ee_x86_register_type_t type, ee_size_t index, ee_size_t size_bits);
ee_bool_t ee_prv_x86_state_append_x87reg_operand(ee_prv_x86_state_t* distate, ee_size_t index);
ee_bool_t ee_prv_x86_state_append_pointer_operand_as_is(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr, ee_x86_segment_register_type_t seg_reg, ee_x86_gpp_register_type_t base_reg);
ee_bool_t ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(ee_prv_x86_state_t* distate, ee_bool_t allow_qword_ptr_promotion, ee_x86_pointer_type_t opt_ptr, ee_x86_segment_register_type_t opt_seg_reg, const ee_prv_x86_modrm_reg_pack_t* base_reg_pack);
ee_bool_t ee_prv_x86_state_append_far_pointer_operand(ee_prv_x86_state_t* distate, ee_uint16_t selector, ee_uint32_t offset, ee_size_t offset_size_bits);
ee_bool_t ee_prv_x86_state_append_imm_bytes_operand(ee_prv_x86_state_t* distate, ee_size_t imm_num_bits, ee_int64_t imm_value);
ee_bool_t ee_prv_x86_state_process_and_append_imm_bytes_operand(ee_prv_x86_state_t* distate, ee_size_t imm_bits);
ee_bool_t ee_prv_x86_state_process_and_append_is4_numreg_operand(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits);
ee_bool_t ee_prv_x86_state_process_and_append_rel_address_operand(ee_prv_x86_state_t* distate, ee_size_t rel_addr_bits);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_rm_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_m_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_bool_t reverse_order);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_bool_t reverse_order);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_vexVVVV_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_size_t vsib_index_reg_size_bits, ee_uint8_t vex_reg_spec);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order);
ee_bool_t ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_uint8_t vex_reg_spec, ee_bool_t reverse_vexVVVV_rm_order);
ee_bool_t ee_prv_x86_state_process_and_append_plusi_x87reg_operand(ee_prv_x86_state_t* distate);
ee_bool_t ee_prv_x86_state_process_and_append_plusr_gppreg_operand(ee_prv_x86_state_t* distate, ee_size_t reg_size_bits);
ee_bool_t ee_prv_x86_state_process_and_append_moffs_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr_type);

#endif
