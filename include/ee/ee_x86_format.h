#ifndef EE_X86_FORMAT_IG
#define EE_X86_FORMAT_IG

#include "ee/ee_x86.h"

/* Functions
*/

ee_bool_t ee_x86_format(ee_x86_mode_t mode, ee_uint64_t instruction_address, const ee_x86_disasm_output_t* disasm_output,
    ee_ascii_char_t* format, ee_size_t* format_size);

/* Implementation
*/

#ifdef EE_X86_FORMAT_IMPL
    #define EE_PRV_UNLOCK_DETAIL
    #include "ee/detail/x86f/format_impl.h"
    #undef EE_PRV_UNLOCK_DETAIL
#endif

#endif
