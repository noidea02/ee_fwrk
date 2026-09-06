#ifndef EE_X86_FORMAT_IG
#define EE_X86_FORMAT_IG

#include "ee/ee_x86.h"

/* Types
*/

/*
This callback gets called whenever a potential memory address (relative address operand, imm32/64 operand, ModR/M
absolute address) is encountered by the formatter. In this case the callee is given the opportunity to provide a
textual (symbolized) representation of the address.

address: The potential address to symbolize.
is_address: EE_TRUE if the formatter is certain that the value is an address, EE_FALSE if not.
symbol: Buffer for storing the symbol.
symbol_size: Size of the symbol buffer in characters (incl. null terminator).
return: EE_TRUE if a symbol was provided, EE_FALSE if not.
*/
typedef ee_bool_t (*ee_x86_symbolize_address_t)(ee_uint64_t address, ee_bool_t is_address, ee_ascii_char_t* symbol, ee_size_t symbol_size);

typedef struct {

    ee_ascii_char_t prefixes[2][16];
    ee_size_t num_prefixes;
    ee_ascii_char_t instruction[24];
    ee_ascii_char_t operands[4][128];
    ee_size_t num_operands;

} ee_x86_tokens_t;

/* Functions
*/

ee_bool_t ee_x86_format(ee_x86_mode_t mode, ee_uint64_t instruction_address, const ee_x86_disasm_output_t* disasm_output,
    ee_x86_symbolize_address_t opt_symbolize_address, ee_ascii_char_t* format, ee_size_t* format_size);

ee_bool_t ee_x86_format_into_tokens(ee_x86_mode_t mode, ee_uint64_t instruction_address, const ee_x86_disasm_output_t* disasm_output,
    ee_x86_symbolize_address_t opt_symbolize_address, ee_x86_tokens_t* tokens);

/* Implementation
*/

#ifdef EE_X86_FORMAT_IMPL
    #define EE_PRV_UNLOCK_DETAIL
    #include "ee/detail/x86f/format_impl.h"
    #undef EE_PRV_UNLOCK_DETAIL
#endif

#endif
