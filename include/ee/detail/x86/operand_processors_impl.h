#ifndef EE_X86_OPERAND_PROCESSORS_IMPL_IG
#define EE_X86_OPERAND_PROCESSORS_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/modrm_tables_impl.h"
#include "ee/detail/x86/state_machine.h"

static ee_bool_t ee_prv_x86_process_operands_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_imm16(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 16);
}

static ee_bool_t ee_prv_x86_process_operands_imm16x32(ee_prv_x86_state_t* distate) {

    ee_size_t imm_bits = 32;

    if (!ee_prv_x86_state_has_active_rex_w(distate) && ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        imm_bits = 16;

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, imm_bits);
}

static ee_bool_t ee_prv_x86_process_operands_imm16_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 16)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_cs(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_CS);
}

static ee_bool_t ee_prv_x86_process_operands_ds(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_DS);
}

static ee_bool_t ee_prv_x86_process_operands_es(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_ES);
}

static ee_bool_t ee_prv_x86_process_operands_fs(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_FS);
}

static ee_bool_t ee_prv_x86_process_operands_gs(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_GS);
}

static ee_bool_t ee_prv_x86_process_operands_ss(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_segreg_operand(distate, EE_X86_SEGMENT_REGISTER_SS);
}

static ee_bool_t ee_prv_x86_process_operands_ax(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AX);
}

static ee_bool_t ee_prv_x86_process_operands_eax(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_EAX);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOraxMODEpASO(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_addr_size_override = ee_prv_x86_state_has_active_address_size_override(distate);
    ee_x86_gpp_register_type_t reg_type = EE_X86_GPP_REGISTER_NOT_EXISTING;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_addr_size_override)
            reg_type = EE_X86_GPP_REGISTER_EAX;
        else
            reg_type = EE_X86_GPP_REGISTER_RAX;
    }
    else {

        if (ee_prv_x86_state_hints_at_16_bit_address_usage(distate))
            reg_type = EE_X86_GPP_REGISTER_AX;
        else
            reg_type = EE_X86_GPP_REGISTER_EAX;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg_type);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOraxMODEpASO_ecx(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_addr_size_override = ee_prv_x86_state_has_active_address_size_override(distate);
    ee_x86_gpp_register_type_t reg_type = EE_X86_GPP_REGISTER_NOT_EXISTING;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_addr_size_override)
            reg_type = EE_X86_GPP_REGISTER_EAX;
        else
            reg_type = EE_X86_GPP_REGISTER_RAX;
    }
    else {

        if (ee_prv_x86_state_hints_at_16_bit_address_usage(distate))
            reg_type = EE_X86_GPP_REGISTER_AX;
        else
            reg_type = EE_X86_GPP_REGISTER_EAX;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg_type)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_ECX);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOraxMODEpASO_cxOecxOrcxMODE_dxOedxOrdxMODE(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_addr_size_override = ee_prv_x86_state_has_active_address_size_override(distate);
    ee_x86_gpp_register_type_t reg0_type = EE_X86_GPP_REGISTER_NOT_EXISTING;
    ee_x86_gpp_register_type_t reg1_type = EE_X86_GPP_REGISTER_NOT_EXISTING;
    ee_x86_gpp_register_type_t reg2_type = EE_X86_GPP_REGISTER_NOT_EXISTING;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_addr_size_override)
            reg0_type = EE_X86_GPP_REGISTER_EAX;
        else
            reg0_type = EE_X86_GPP_REGISTER_RAX;

        reg1_type = EE_X86_GPP_REGISTER_RCX;
        reg2_type = EE_X86_GPP_REGISTER_RDX;
    }
    else if(ee_prv_x86_state_is_16_bit_mode_active(distate)) {

        if (has_addr_size_override)
            reg0_type = EE_X86_GPP_REGISTER_EAX;
        else
            reg0_type = EE_X86_GPP_REGISTER_AX;

        reg1_type = EE_X86_GPP_REGISTER_CX;
        reg2_type = EE_X86_GPP_REGISTER_DX;
    }
    else {

        if (has_addr_size_override)
            reg0_type = EE_X86_GPP_REGISTER_AX;
        else
            reg0_type = EE_X86_GPP_REGISTER_EAX;

        reg1_type = EE_X86_GPP_REGISTER_ECX;
        reg2_type = EE_X86_GPP_REGISTER_EDX;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg0_type)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg1_type)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg2_type);
}

static ee_bool_t ee_prv_x86_process_operands_byte_ptr_bxOebxOrbx(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_addr_size_override = ee_prv_x86_state_has_active_address_size_override(distate);
    const ee_x86_segment_register_type_t seg_reg = ee_prv_x86_prefix_state_to_segment_register(&distate->active_prefixes);
    ee_x86_gpp_register_type_t base_reg = EE_X86_GPP_REGISTER_NOT_EXISTING;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_addr_size_override)
            base_reg = EE_X86_GPP_REGISTER_EBX;
        else
            base_reg = EE_X86_GPP_REGISTER_RBX;
    }
    else if (ee_prv_x86_state_is_16_bit_mode_active(distate)) {

        if (has_addr_size_override)
            base_reg = EE_X86_GPP_REGISTER_EBX;
        else
            base_reg = EE_X86_GPP_REGISTER_BX;
    }
    else {

        if (has_addr_size_override)
            base_reg = EE_X86_GPP_REGISTER_BX;
        else
            base_reg = EE_X86_GPP_REGISTER_EBX;
    }

    return ee_prv_x86_state_append_pointer_operand_as_is(distate, EE_X86_POINTER_BYTE, seg_reg, base_reg);
}

static ee_bool_t ee_prv_x86_process_operands_al_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_imm8_al(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL);
}

static ee_bool_t ee_prv_x86_process_operands_al_dx(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX);
}

static ee_bool_t ee_prv_x86_process_operands_dx_al(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL);
}

static ee_bool_t ee_prv_x86_process_operands_al_moffs8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL)
        && ee_prv_x86_state_process_and_append_moffs_operand(distate, EE_X86_POINTER_BYTE);
}

static ee_bool_t ee_prv_x86_process_operands_moffs8_al(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_moffs_operand(distate, EE_X86_POINTER_BYTE)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL);
}

static ee_bool_t ee_prv_x86_process_operands_axOeax_imm8(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t gpp_reg = EE_X86_GPP_REGISTER_EAX;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate))
        gpp_reg = EE_X86_GPP_REGISTER_AX;

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, gpp_reg)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_imm8_axOeax(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t gpp_reg = EE_X86_GPP_REGISTER_EAX;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate))
        gpp_reg = EE_X86_GPP_REGISTER_AX;

    return ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, gpp_reg);
}

static ee_bool_t ee_prv_x86_process_operands_axOeax_dx(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t gpp_reg = EE_X86_GPP_REGISTER_EAX;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate))
        gpp_reg = EE_X86_GPP_REGISTER_AX;

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, gpp_reg)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX);
}

static ee_bool_t ee_prv_x86_process_operands_dx_axOeax(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t gpp_reg = EE_X86_GPP_REGISTER_EAX;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate))
        gpp_reg = EE_X86_GPP_REGISTER_AX;

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, gpp_reg);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOraxPREF_imm16x32(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t reg_type = EE_X86_GPP_REGISTER_NOT_EXISTING;
    ee_size_t imm_bits = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        reg_type = EE_X86_GPP_REGISTER_RAX;
        imm_bits = 32;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        reg_type = EE_X86_GPP_REGISTER_AX;
        imm_bits = 16;
    }
    else {

        reg_type = EE_X86_GPP_REGISTER_EAX;
        imm_bits = 32;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg_type)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, imm_bits);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOraxPREF_moffs16x32x64(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t reg = EE_X86_GPP_REGISTER_EAX;
    ee_x86_pointer_type_t ptr = EE_X86_POINTER_DWORD;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        reg = EE_X86_GPP_REGISTER_RAX;
        ptr = EE_X86_POINTER_QWORD;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        reg = EE_X86_GPP_REGISTER_AX;
        ptr = EE_X86_POINTER_WORD;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg)
        && ee_prv_x86_state_process_and_append_moffs_operand(distate, ptr);
}

static ee_bool_t ee_prv_x86_process_operands_moffs16x32x64_axOeaxOraxPREF(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t reg = EE_X86_GPP_REGISTER_EAX;
    ee_x86_pointer_type_t ptr = EE_X86_POINTER_DWORD;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        reg = EE_X86_GPP_REGISTER_RAX;
        ptr = EE_X86_POINTER_QWORD;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        reg = EE_X86_GPP_REGISTER_AX;
        ptr = EE_X86_POINTER_WORD;
    }

    return ee_prv_x86_state_process_and_append_moffs_operand(distate, ptr)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, reg);
}

static ee_bool_t ee_prv_x86_process_operands_rel8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_rel_address_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_rel16x32X86rel32X64(ee_prv_x86_state_t* distate) {

    ee_size_t rel_bits = 32;

    if (!ee_prv_x86_state_is_64_bit_mode_active(distate) && ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        rel_bits = 16;

    return ee_prv_x86_state_process_and_append_rel_address_operand(distate, rel_bits);
}

static ee_bool_t ee_prv_x86_process_operands_al_m8AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOrax_m16x32x64AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_with_prefix_adjustments(distate, &EE_PRV_X86_MODRM_REG_PACK_EAX)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_al_m8AesAdiOediOrdi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI);
}

static ee_bool_t ee_prv_x86_process_operands_m8AesAdiOediOrdi_al(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_AL);
}

static ee_bool_t ee_prv_x86_process_operands_axOeaxOrax_m16x32x64AesAdiOediOrdi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_with_prefix_adjustments(distate, &EE_PRV_X86_MODRM_REG_PACK_EAX)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI);
}

static ee_bool_t ee_prv_x86_process_operands_m16x32x64AesAdiOediOrdi_axOeaxOrax(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_gppreg_operand_with_prefix_adjustments(distate, &EE_PRV_X86_MODRM_REG_PACK_EAX);
}

static ee_bool_t ee_prv_x86_process_operands_dx_m8AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_dx_m16x32AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_m8AesAdiOediOrdi_dx(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX);
}

static ee_bool_t ee_prv_x86_process_operands_m16x32AesAdiOediOrdi_dx(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_DX);
}

static ee_bool_t ee_prv_x86_process_operands_m8AsiOesiOrsi_m8AesAdiOediOrdi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI);
}

static ee_bool_t ee_prv_x86_process_operands_m8AesAdiOediOrdi_m8AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_FALSE, EE_X86_POINTER_BYTE, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_m16x32x64AsiOesiOrsi_m16x32x64AesAdiOediOrdi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI);
}

static ee_bool_t ee_prv_x86_process_operands_m16x32x64AesAdiOediOrdi_m16x32x64AsiOesiOrsi(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_ES, &EE_PRV_X86_MODRM_REG_PACK_EDI)
        && ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(distate, EE_TRUE, EE_X86_POINTER_NOT_EXISTING, EE_X86_SEGMENT_REGISTER_NOT_EXISTING, &EE_PRV_X86_MODRM_REG_PACK_ESI);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_NOT_EXISTING);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_BYTE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_DWORD);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_QWORD);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m128(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_OWORD);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m512(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, EE_X86_POINTER_NOT_EXISTING);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m48x80MODE(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate))
        ptr_type = EE_X86_POINTER_TBYTE;
    else
        ptr_type = EE_X86_POINTER_FWORD;

    return ee_prv_x86_state_process_and_append_modrm_m_operand(distate, ptr_type);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_m32x64_r32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, ptr_type, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm8_1(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 8)
        && ee_prv_x86_state_append_imm_bytes_operand(distate, 8, 1);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm8_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 8)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm8_cl(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 8)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_CL);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, 16);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x48x80(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_FWORD;
    ee_size_t reg_size_bits = 48;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        ptr_type = EE_X86_POINTER_TBYTE;
        reg_size_bits = 80;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        ptr_type = EE_X86_POINTER_DWORD;
        reg_size_bits = 32;
    }

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32M32x64M64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate)) {

            ptr_type = EE_X86_POINTER_WORD;
            reg_size_bits = 16;
        }
        else {

            ptr_type = EE_X86_POINTER_QWORD;
            reg_size_bits = 64;
        }
    }
    else {

        if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

            ptr_type = EE_X86_POINTER_WORD;
            reg_size_bits = 16;
        }
        else {

            ptr_type = EE_X86_POINTER_DWORD;
            reg_size_bits = 32;
        }
    }

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32M32o16x64M64(ee_prv_x86_state_t* distate) {

    const ee_bool_t use_16_bit_operand = ee_prv_x86_state_hints_at_16_bit_operand_usage(distate);
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_WORD;
    ee_size_t reg_size_bits = 16;

    if (!ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (!use_16_bit_operand) {

            ptr_type = EE_X86_POINTER_DWORD;
            reg_size_bits = 32;
        }
    }
    else {

        if (!use_16_bit_operand || ee_prv_x86_state_has_active_rex_w(distate)) {

            ptr_type = EE_X86_POINTER_QWORD;
            reg_size_bits = 64;
        }
    }

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_DWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_QWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 64);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm80(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_TBYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 80);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_1(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits)
        && ee_prv_x86_state_append_imm_bytes_operand(distate, 8, 1);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_imm8(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_imm16x32(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;
    ee_size_t imm_bits = 32;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    if (reg_size_bits == 16)
        imm_bits = 16;

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, imm_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_cl(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_CL);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64m16(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ignored_ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_sreg_rm16x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_WORD;
    ee_size_t rm_reg_size_bits = 16;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_SEGMENT,
        16,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64m16_sreg(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ignored_rm_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t rm_reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ignored_rm_ptr_type, &rm_reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_SEGMENT,
        16,
        EE_X86_POINTER_WORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_rm8(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_BYTE,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        8,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_rm32x64(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;
    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {
        
        reg_size_bits = 64;
        rm_ptr_type = EE_X86_POINTER_QWORD;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_rm16x32x64(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;
    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t rm_reg_size_bits = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &rm_ptr_type, &rm_reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm8_r8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, 8, EE_X86_POINTER_BYTE, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16_r16(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, 16, EE_X86_POINTER_WORD, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r8_rm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, 8, EE_X86_POINTER_BYTE, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_r16x32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, ptr_type, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_r16x32x64_cl(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_rm16x32x64_r16x32x64(distate)
        && ee_prv_x86_state_append_gppreg_operand_as_is(distate, EE_X86_GPP_REGISTER_CL);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm16x32x64_r16x32x64_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_rm16x32x64_r16x32x64(distate)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32_m32x48(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_FWORD;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        ptr_type = EE_X86_POINTER_DWORD;
        reg_size_bits = 16;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32_m32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_QWORD;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        ptr_type = EE_X86_POINTER_DWORD;
        reg_size_bits = 16;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm32(ee_prv_x86_state_t* distate) {

    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        rm_reg_size_bits = 64;
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        rm_reg_size_bits = 16;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_X86_POINTER_DWORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        32,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_m(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ignored_ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_m32x48x80(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    switch (reg_size_bits) {
    case 16: ptr_type = EE_X86_POINTER_DWORD; break;
    case 32: ptr_type = EE_X86_POINTER_FWORD; break;
    case 64: ptr_type = EE_X86_POINTER_TBYTE; break;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm8(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ignored_ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_BYTE,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        8,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm16(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ignored_ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_WORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        16,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm16x32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, ptr_type, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_r16x32x64m16(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    ee_prv_x86_state_get_dynamic_modrm_operand_info(distate, &ptr_type, &reg_size_bits);
    ptr_type = EE_X86_POINTER_WORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, ptr_type, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm16x32x64_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_r16x32x64_rm16x32x64(distate)
        && ee_prv_x86_process_operands_imm8(distate);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64_rm16x32x64_imm16x32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_r16x32x64_rm16x32x64(distate)
        && ee_prv_x86_process_operands_imm16x32(distate);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64MODE_rm32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;

    if (reg_size_bits == 64)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits != 32)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64MODE_r32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;

    if (reg_size_bits == 64)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits != 32)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(
        distate,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64MODE_m128(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_OWORD,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64MODEpASO(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_aso = ee_prv_x86_state_has_active_address_size_override(distate);
    ee_size_t reg_size_bits = 0;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_aso)
            reg_size_bits = 32;
        else
            reg_size_bits = 64;
    }
    else if (ee_prv_x86_state_is_16_bit_mode_active(distate)) {

        if (has_aso)
            reg_size_bits = 32;
        else
            reg_size_bits = 16;
    }
    else {

        if (has_aso)
            reg_size_bits = 16;
        else
            reg_size_bits = 32;
    }

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(
        distate,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r16x32x64MODEpASO_m512(ee_prv_x86_state_t* distate) {

    const ee_bool_t has_aso = ee_prv_x86_state_has_active_address_size_override(distate);
    ee_size_t reg_size_bits = 0;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (has_aso)
            reg_size_bits = 32;
        else
            reg_size_bits = 64;
    }
    else if (ee_prv_x86_state_is_16_bit_mode_active(distate)) {

        if (has_aso)
            reg_size_bits = 32;
        else
            reg_size_bits = 16;
    }
    else {

        if (has_aso)
            reg_size_bits = 16;
        else
            reg_size_bits = 32;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_ZMMWORD,
        EE_X86_REGISTER_NOT_EXISTING,
        0,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64MODE_cr(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_CONTROL,
        reg_size_bits,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_cr_r32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_CONTROL,
        reg_size_bits,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64MODE_dr(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_DEBUG,
        reg_size_bits,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_dr_r32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_DEBUG,
        reg_size_bits,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64MODE_bnd(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;

    if (reg_size_bits == 64)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits != 32)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_brm64x128MODE_bnd(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 128 : 64;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_QWORD;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_OWORD;
    else if (reg_size_bits != 64)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_bnd_rm32x64MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 64 : 32;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;

    if (reg_size_bits == 64)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits != 32)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_bnd_brm64x128MODE(ee_prv_x86_state_t* distate) {

    const ee_size_t reg_size_bits = ee_prv_x86_state_is_64_bit_mode_active(distate) ? 128 : 64;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_QWORD;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_OWORD;
    else if (reg_size_bits != 64)
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_MPX,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        32,
        EE_X86_POINTER_NOT_EXISTING,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32_xymm(ee_prv_x86_state_t* distate) {

    ee_uint8_t ignored_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type;
    ee_size_t reg_size_bits;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &ignored_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        32,
        EE_X86_POINTER_NOT_EXISTING,
        reg_type,
        reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_NOT_EXISTING, EE_X86_REGISTER_MMX, 64)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_mm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;
    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, EE_X86_POINTER_NOT_EXISTING, EE_X86_REGISTER_MMX, 64, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64_mm(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_mrm32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        EE_X86_POINTER_DWORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_mrm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        EE_X86_POINTER_MMWORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mrm64_mm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        EE_X86_POINTER_MMWORD,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_r32m16_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_MMX, 64, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_mrm64_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_MMX, 64, EE_X86_POINTER_MMWORD, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_xrm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        EE_X86_POINTER_MMWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_mm_xrm128(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_MMX,
        64,
        EE_X86_POINTER_XMMWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_rm32x64_imm8(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, rm_ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_mrm64(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_MMWORD,
        EE_X86_REGISTER_MMX,
        64,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_xrm32(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_DWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_xrm64(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_QWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_xrm128(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        EE_X86_POINTER_XMMWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_xyrm128x256(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;
    ee_uint8_t ignored_vex_reg_spec = 0;
    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t rm_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t rm_reg_size_bits = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &ignored_vex_reg_spec, &rm_ptr_type, &rm_reg_type, &rm_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        rm_ptr_type,
        rm_reg_type,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, EE_X86_POINTER_NOT_EXISTING, EE_X86_REGISTER_AVX_128, 128)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_r32m8_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_r32m16_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_rm32x64_imm8(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, rm_ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, rm_reg_size_bits, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_xmm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;
    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, EE_X86_POINTER_NOT_EXISTING, EE_X86_REGISTER_AVX_128, 128, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64_xmm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t rm_reg_size_bits = 32;
    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_reg_size_bits = 64;
        rm_ptr_type = EE_X86_POINTER_QWORD;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, rm_ptr_type, EE_X86_REGISTER_GENERAL_PURPOSE, rm_reg_size_bits, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32m8_xmm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32m16_xmm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32m32_xmm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r64m64_xmm_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_QWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 64, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64m8_xmm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        rm_reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, rm_reg_size_bits, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64m16_xmm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        rm_reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, rm_reg_size_bits, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64m32_xmm_imm8(ee_prv_x86_state_t* distate) {

    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        rm_reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, EE_X86_REGISTER_GENERAL_PURPOSE, rm_reg_size_bits, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_rm32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_DWORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        32,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_DWORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        32,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_rm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_QWORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        64,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm64_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_QWORD,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        64,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_mrm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_MMWORD,
        EE_X86_REGISTER_MMX,
        64,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_rm32x64_xmm(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm16(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_WORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm32(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_DWORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm32_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_DWORD,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm64(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_QWORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm64_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_QWORD,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm128(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_XMMWORD,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm128_xmm(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_X86_POINTER_XMMWORD,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_m128(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_XMMWORD,
        EE_X86_REGISTER_NOT_EXISTING,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_BYTE,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm16(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_WORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm16x32(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_WORD;
    else if (reg_size_bits == 256)
        ptr_type = EE_X86_POINTER_DWORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm32x64(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_DWORD;
    else if (reg_size_bits == 256)
        ptr_type = EE_X86_POINTER_QWORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm64x128(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits == 256)
        ptr_type = EE_X86_POINTER_XMMWORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm64x128_xymm_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_QWORD;
    else if (reg_size_bits == 256)
        ptr_type = EE_X86_POINTER_XMMWORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, reg_type, reg_size_bits, ptr_type, EE_X86_REGISTER_AVX_128, 128, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm32(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_DWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xrm64(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_QWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xyrm64x256(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    if (reg_size_bits == 128)
        ptr_type = EE_X86_POINTER_QWORD;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xyrm128x256(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t rm_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t rm_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &rm_reg_type, &rm_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        ptr_type,
        rm_reg_type,
        rm_reg_size_bits,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xyrm128x256(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xyrm128x256_xymm(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        EE_TRUE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xyrm128x256_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_xymm_xyrm128x256(distate)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymmVVVV_xyrm128x256_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands(distate, reg_type, reg_size_bits, ptr_type, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm32_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm64_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_QWORD, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm128_xmm0(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_XMMWORD, EE_FALSE)
        && ee_prv_x86_state_append_numreg_operand(distate, EE_X86_REGISTER_AVX_128, 0, 128);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xrm128_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_XMMWORD, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm128_xymm_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(distate, reg_type, reg_size_bits, EE_X86_POINTER_XMMWORD, EE_X86_REGISTER_AVX_128, 128, EE_TRUE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t rm_reg_size_bits = 32;
    ee_uint8_t vex_reg_spec = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        rm_reg_size_bits = 64;
    }

    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_AVX_128,
        128,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        rm_reg_size_bits,
        vex_reg_spec,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_xrm32(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, vex_reg_spec, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_xrm64(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_QWORD, vex_reg_spec, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_xrm128(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_XMMWORD, vex_reg_spec, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xrm128_xmmVVVV_xmm(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_XMMWORD, vex_reg_spec, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xyrm64(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, reg_type, reg_size_bits, EE_X86_POINTER_QWORD, vex_reg_spec, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xrm128(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        EE_X86_POINTER_XMMWORD,
        EE_X86_REGISTER_AVX_128,
        128,
        vex_reg_spec,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xyrm128x256(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, reg_type, reg_size_bits, ptr_type, vex_reg_spec, EE_FALSE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xyrm128x256_xymmVVVV_xymm(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, reg_type, reg_size_bits, ptr_type, vex_reg_spec, EE_TRUE);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_m32vm64xy_xmmVVVV(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, reg_size_bits, vex_reg_spec);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_m32vm32xy_xymmVVVV(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(distate, reg_type, reg_size_bits, EE_X86_POINTER_DWORD, reg_size_bits, vex_reg_spec);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_m64vm32x_xymmVVVV(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(distate, reg_type, reg_size_bits, EE_X86_POINTER_QWORD, 128, vex_reg_spec);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_m64vm64xy_xymmVVVV(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(distate, reg_type, reg_size_bits, EE_X86_POINTER_QWORD, reg_size_bits, vex_reg_spec);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_r32m8_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_BYTE, EE_X86_REGISTER_GENERAL_PURPOSE, 32, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_r32m16_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_WORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_rm32_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 32, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_rm64_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_QWORD, EE_X86_REGISTER_GENERAL_PURPOSE, 64, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_xrm32_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_DWORD, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xmm_xmmVVVV_xrm64_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, EE_X86_REGISTER_AVX_128, 128, EE_X86_POINTER_QWORD, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xrm128_imm8(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ignored_ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(distate, reg_type, reg_size_bits, EE_X86_POINTER_XMMWORD, EE_X86_REGISTER_AVX_128, 128, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xyrm128x256_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xyrm128x256(distate)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_xymm_xymmVVVV_xyrm128x256_xymmIS4(ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_reg_spec = 0;
    ee_x86_pointer_type_t ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t reg_size_bits = 0;

    if (!ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, &vex_reg_spec, &ptr_type, &reg_type, &reg_size_bits))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(distate, reg_type, reg_size_bits, ptr_type, vex_reg_spec, EE_FALSE)
        && ee_prv_x86_state_process_and_append_is4_numreg_operand(distate, reg_type, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64VVVV_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;
    ee_uint8_t vex_reg_spec = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands_fine_grained(
        distate,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        vex_reg_spec
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_r32x64VVVV_rm32x64(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;
    ee_uint8_t vex_reg_spec = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        vex_reg_spec,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_r32x64_rm32x64_r32x64VVVV(ee_prv_x86_state_t* distate) {

    ee_x86_pointer_type_t rm_ptr_type = EE_X86_POINTER_DWORD;
    ee_size_t reg_size_bits = 32;
    ee_uint8_t vex_reg_spec = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        rm_ptr_type = EE_X86_POINTER_QWORD;
        reg_size_bits = 64;
    }

    if (!ee_prv_x86_state_vex_get_register_specifier(distate, &vex_reg_spec))
        return EE_FALSE;

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_vexVVVV_operands_fine_grained(
        distate,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        rm_ptr_type,
        EE_X86_REGISTER_GENERAL_PURPOSE,
        reg_size_bits,
        vex_reg_spec,
        EE_FALSE
    );
}

static ee_bool_t ee_prv_x86_process_operands_modrm_fake_imm8(ee_prv_x86_state_t* distate) {

    /* "modrm_fake" operand processors must only be used in conjunction with operand range constraints. In this case the state
    *  machine automatically skips the fake ModR/M byte.
    */

    return ee_prv_x86_process_operands_imm8(distate);
}

static ee_bool_t ee_prv_x86_process_operands_modrm_fake_rel16x32(ee_prv_x86_state_t* distate) {
    
    return ee_prv_x86_process_operands_rel16x32X86rel32X64(distate);
}

static ee_bool_t ee_prv_x86_process_operands_plusi_sti(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_plusi_x87reg_operand(distate);
}

static ee_bool_t ee_prv_x86_process_operands_plusi_st0_sti(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_append_x87reg_operand(distate, EE_X86_DEFAULT_NUMBERED_REGISTER_INDEX)
        && ee_prv_x86_state_process_and_append_plusi_x87reg_operand(distate);
}

static ee_bool_t ee_prv_x86_process_operands_plusi_sti_st0(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_plusi_x87reg_operand(distate)
        && ee_prv_x86_state_append_x87reg_operand(distate, EE_X86_DEFAULT_NUMBERED_REGISTER_INDEX);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_r16x32(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        reg_size_bits = 16;

    return ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_r16x32x64(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 64;
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        reg_size_bits = 16;

    return ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_r16PREFx32x64MODE(ee_prv_x86_state_t* distate) {

    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate) && !ee_prv_x86_state_has_active_rex_w(distate))
        reg_size_bits = 16;
    else if (ee_prv_x86_state_is_64_bit_mode_active(distate))
        reg_size_bits = 64;

    return ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_axOeaxOrax_r16x32x64(ee_prv_x86_state_t* distate) {

    ee_x86_gpp_register_type_t dst_reg = EE_X86_GPP_REGISTER_EAX;
    ee_size_t reg_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        dst_reg = EE_X86_GPP_REGISTER_RAX;
        reg_size_bits = 64;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        dst_reg = EE_X86_GPP_REGISTER_AX;
        reg_size_bits = 16;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, dst_reg)
        && ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, reg_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_r8_imm8(ee_prv_x86_state_t* distate) {

    return ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, 8)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, 8);
}

static ee_bool_t ee_prv_x86_process_operands_plusr_r16x32x64_imm16x32x64(ee_prv_x86_state_t* distate) {

    ee_size_t data_size_bits = 32;

    if (ee_prv_x86_state_has_active_rex_w(distate))
        data_size_bits = 64;
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate))
        data_size_bits = 16;

    return ee_prv_x86_state_process_and_append_plusr_gppreg_operand(distate, data_size_bits)
        && ee_prv_x86_state_process_and_append_imm_bytes_operand(distate, data_size_bits);
}

static ee_bool_t ee_prv_x86_process_operands_farptr16A16x32(ee_prv_x86_state_t* distate) {

    ee_size_t offset_size_bits = 32;
    ee_uint64_t offset_bit_mask = 0xffffffff;

    ee_int64_t raw_ptr = 0;
    ee_uint16_t selector = 0;
    ee_uint32_t offset = 0;

    if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        offset_size_bits = 16;
        offset_bit_mask = 0xffff;
    }

    if (!ee_prv_x86_state_consume_immediate_bytes(distate, 16 + offset_size_bits, &raw_ptr))
        return EE_FALSE;

    selector = (ee_uint16_t)(raw_ptr >> offset_size_bits);
    offset = (ee_uint32_t)(raw_ptr & offset_bit_mask);

    return ee_prv_x86_state_append_far_pointer_operand(distate, selector, offset, offset_size_bits);
}

#endif
