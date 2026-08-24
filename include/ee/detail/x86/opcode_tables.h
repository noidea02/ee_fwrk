#ifndef EE_X86_OPCODE_TABLES_IG
#define EE_X86_OPCODE_TABLES_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/operand_constraints.h"
#include "ee/detail/x86/prefix_constraints.h"
#include "ee/detail/x86/state_machine.h"

typedef enum {

    EE_PRV_X86_SUPPMOD_NONE,
    EE_PRV_X86_SUPPMOD_32,
    EE_PRV_X86_SUPPMOD_64,
    EE_PRV_X86_SUPPMOD_32_64

} ee_prv_x86_supported_mode_t;

typedef struct {

    ee_prv_x86_supported_mode_t supported_mode;
    const ee_prv_x86_prefix_constraints_t* prefix_constraints;
    ee_x86_instruction_type_t instruction;
    const ee_prv_x86_operand_constraints_t* operand_constraints;
    ee_prv_x86_process_operand_bytes_t process_operand_bytes;
    ee_uint32_t flags;

} ee_prv_x86_opcode_identity_t;

typedef struct {

    const ee_prv_x86_opcode_identity_t* elements;
    ee_size_t num_elements;

} ee_prv_x86_opcode_identity_list_t;

#endif
