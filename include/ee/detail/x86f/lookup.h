#ifndef EE_X86F_LOOKUP_IG
#define EE_X86F_LOOKUP_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_x86.h"

const ee_ascii_char_t* ee_prv_x86f_lookup_prefix_str(ee_x86_prefix_type_t pref, ee_ascii_char_t* out, ee_size_t out_size);
const ee_ascii_char_t* ee_prv_x86f_lookup_instruction_str(ee_x86_instruction_type_t inst, ee_ascii_char_t* out, ee_size_t out_size);
const ee_ascii_char_t* ee_prv_x86f_lookup_gpp_register_str(ee_x86_gpp_register_type_t gpp_reg, ee_ascii_char_t* out, ee_size_t out_size);
const ee_ascii_char_t* ee_prv_x86f_lookup_segment_register_str(ee_x86_segment_register_type_t seg_reg, ee_ascii_char_t* out, ee_size_t out_size);
const ee_ascii_char_t* ee_prv_x86f_lookup_pointer_type_str(ee_x86_pointer_type_t ptr_type, ee_ascii_char_t* out, ee_size_t out_size);

#endif
