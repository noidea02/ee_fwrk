#ifndef EE_X86_STATE_MACHINE_IMPL_IG
#define EE_X86_STATE_MACHINE_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/constants.h"
#include "ee/detail/x86/instruction_pseudonyms_impl.h"
#include "ee/detail/x86/modrm_tables_impl.h"
#include "ee/detail/x86/opcode_tables_impl.h"
#include "ee/detail/x86/state_machine.h"

EE_STATIC_ASSERT(EE_GET_TDEF_STRUCT_FIELD_LEN(ee_x86_disasm_output_t, prefixes) == EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_x86_state_t, final_prefixes), size_mismatch_for_external_and_internal_ee_x86_prefix_fields);
EE_STATIC_ASSERT(EE_GET_TDEF_STRUCT_FIELD_LEN(ee_x86_disasm_output_t, operands) == EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_x86_state_t, operands), size_mismatch_for_external_and_internal_ee_x86_operand_fields);

static ee_bool_t ee_prv_x86_state_get_current_byte(const ee_prv_x86_state_t* distate, ee_byte_t* cur_byte) {

    if (distate->byte_index >= distate->num_bytes)
        return EE_FALSE;

    *cur_byte = distate->bytes[distate->byte_index];

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_get_next_byte(const ee_prv_x86_state_t* distate, ee_byte_t* next_byte) {

    const ee_size_t next_index = distate->byte_index + 1;
    if (next_index >= distate->num_bytes)
        return EE_FALSE;

    *next_byte = distate->bytes[next_index];

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_advance_bytes(ee_prv_x86_state_t* distate, ee_byte_t* old_byte) {

    if (!ee_prv_x86_state_get_current_byte(distate, old_byte))
        return EE_FALSE;

    ++distate->byte_index;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_just_advance_bytes(ee_prv_x86_state_t* distate) {

    if (distate->byte_index >= distate->num_bytes)
        return EE_FALSE;

    ++distate->byte_index;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_advance_and_get_current_byte(ee_prv_x86_state_t* distate, ee_byte_t* cur_byte) {

    if (!ee_prv_x86_state_just_advance_bytes(distate) || !ee_prv_x86_state_get_current_byte(distate, cur_byte))
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_is_16_bit_mode_active(const ee_prv_x86_state_t* distate) {
    return distate->active_mode == EE_X86_MODE_16;
}

static ee_bool_t ee_prv_x86_state_is_64_bit_mode_active(const ee_prv_x86_state_t* distate) {
    return distate->active_mode == EE_X86_MODE_64;
}

static ee_bool_t ee_prv_x86_state_has_active_operand_size_override(const ee_prv_x86_state_t* distate) {

    const ee_size_t aso_index = 2;
    if (aso_index >= EE_GET_ARRAY_LEN(distate->active_prefixes.grp_prefixes))
        return EE_FALSE;

    return distate->active_prefixes.grp_prefixes[aso_index] == EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR;
}

static ee_bool_t ee_prv_x86_state_has_active_address_size_override(const ee_prv_x86_state_t* distate) {

    const ee_size_t aso_index = 3;
    if (aso_index >= EE_GET_ARRAY_LEN(distate->active_prefixes.grp_prefixes))
        return EE_FALSE;

    return distate->active_prefixes.grp_prefixes[aso_index] == EE_PRV_X86_GRP4_PREFIX_ADR_SIZE_OVR;
}

static ee_bool_t ee_prv_x86_state_dissect_active_vex_prefix(const ee_prv_x86_state_t* distate, ee_uint8_t* normalized_rxbwl_bits, ee_uint8_t* normalized_reg_spec) {

    if (distate->active_prefixes.num_vex_core_bytes == 1) {

        const ee_uint8_t vex_byte_1 = distate->active_prefixes.vex_core_bytes[0];

        const ee_uint8_t r_bit = (vex_byte_1 & EE_PRV_X86_VEX_R_MASK) == 0;
        const ee_uint8_t l_bit = (vex_byte_1 & EE_PRV_X86_VEX_L_MASK) != 0;
        const ee_uint8_t vvvv_bits = (vex_byte_1 & EE_PRV_X86_VEX_VVVV_MASK) >> 3;

        *normalized_rxbwl_bits = (r_bit << 4) | l_bit;
        *normalized_reg_spec = (~vvvv_bits) & 0x0f;

        return EE_TRUE;
    }
    else if (distate->active_prefixes.num_vex_core_bytes == 2) {

        const ee_uint8_t vex_byte_1 = distate->active_prefixes.vex_core_bytes[0];
        const ee_uint8_t vex_byte_2 = distate->active_prefixes.vex_core_bytes[1];

        const ee_uint8_t r_bit = (vex_byte_1 & EE_PRV_X86_VEX_R_MASK) == 0;
        const ee_uint8_t x_bit = (vex_byte_1 & EE_PRV_X86_VEX_X_MASK) == 0;
        const ee_uint8_t b_bit = (vex_byte_1 & EE_PRV_X86_VEX_B_MASK) == 0;
        const ee_uint8_t w_bit = (vex_byte_2 & EE_PRV_X86_VEX_W_MASK) != 0;
        const ee_uint8_t l_bit = (vex_byte_2 & EE_PRV_X86_VEX_L_MASK) != 0;
        const ee_uint8_t vvvv_bits = (vex_byte_2 & EE_PRV_X86_VEX_VVVV_MASK) >> 3;

        *normalized_rxbwl_bits = (r_bit << 4) | (x_bit << 3) | (b_bit << 2) | (w_bit << 1) | l_bit;
        *normalized_reg_spec = (~vvvv_bits) & 0x0f;

        return EE_TRUE;
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_state_has_active_rex_w(const ee_prv_x86_state_t* distate) {

    ee_uint8_t vex_rxbwl = 0;
    ee_uint8_t ignored_vex_reg_spec = 0;

    if (!ee_prv_x86_state_is_64_bit_mode_active(distate))
        return EE_FALSE;

    if ((distate->active_prefixes.rex_prefix & EE_PRV_X86_REX_MIN) && (distate->active_prefixes.rex_prefix & EE_PRV_X86_REX_W_MASK))
        return EE_TRUE;

    if (ee_prv_x86_state_dissect_active_vex_prefix(distate, &vex_rxbwl, &ignored_vex_reg_spec) && (vex_rxbwl & 2))
        return EE_TRUE;

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_state_has_active_vex_prefix(const ee_prv_x86_state_t* distate) {

    return distate->active_prefixes.num_vex_core_bytes > 0;
}

static ee_bool_t ee_prv_x86_state_hints_at_16_bit_operand_usage(const ee_prv_x86_state_t* distate) {

    const ee_bool_t mode_16 = ee_prv_x86_state_is_16_bit_mode_active(distate);
    const ee_bool_t has_opd_size_override = ee_prv_x86_state_has_active_operand_size_override(distate);

    return (mode_16 && !has_opd_size_override) || (!mode_16 && has_opd_size_override);
}

static ee_bool_t ee_prv_x86_state_hints_at_16_bit_address_usage(const ee_prv_x86_state_t* distate) {

    const ee_bool_t mode_16 = ee_prv_x86_state_is_16_bit_mode_active(distate);
    const ee_bool_t has_addr_size_override = ee_prv_x86_state_has_active_address_size_override(distate);

    return (mode_16 && !has_addr_size_override) || (!mode_16 && has_addr_size_override);
}

static ee_bool_t ee_prv_x86_state_must_use_modrm16_table(const ee_prv_x86_state_t* distate) {

    return !ee_prv_x86_state_is_64_bit_mode_active(distate) && ee_prv_x86_state_hints_at_16_bit_address_usage(distate);
}

static const ee_prv_x86_modrm_mapping_t* ee_prv_x86_state_get_modrm_mapping(ee_prv_x86_state_t* distate) {

    ee_byte_t modrm_byte = 0;
    ee_size_t table_index = 0;
    const ee_prv_x86_modrm_mapping_t* result = 0;

    if (!ee_prv_x86_state_advance_bytes(distate, &modrm_byte))
        return 0;

    table_index = modrm_byte;
    if (ee_prv_x86_state_must_use_modrm16_table(distate)) {

        if (table_index >= EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM16_TABLE))
            return 0;

        result = &EE_PRV_X86_MODRM16_TABLE[table_index];
    }
    else {

        if (table_index >= EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM32_TABLE))
            return 0;

        result = &EE_PRV_X86_MODRM32_TABLE[table_index];
    }

    return result;
}

static const ee_prv_x86_modrm32_sib_mapping_t* ee_prv_x86_state_get_modrm32_sib_mapping(ee_prv_x86_state_t* distate, ee_bool_t requires_vsib) {

    ee_byte_t sib = 0;
    ee_size_t table_count = 0;
    const ee_prv_x86_modrm32_sib_mapping_t* table = 0;

    if (!ee_prv_x86_state_advance_bytes(distate, &sib))
        return 0;

    if (requires_vsib) {

        table_count = EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM32_VSIB_TABLE);
        table = EE_PRV_X86_MODRM32_VSIB_TABLE;
    }
    else {

        table_count = EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM32_SIB_TABLE);
        table = EE_PRV_X86_MODRM32_SIB_TABLE;
    }

    if (sib >= table_count)
        return 0;

    return &table[sib];
}

static ee_bool_t ee_prv_x86_prefix_state_are_rex_fields_set(const ee_prv_x86_prefix_state_t* ps, ee_uint8_t bits) {

    if (!bits)
        return EE_FALSE;

    if (ps->rex_prefix >= EE_PRV_X86_REX_MIN) {

        const ee_bool_t is_set = (ps->rex_prefix & bits) == bits;
        return is_set;
    }
    else if (ps->num_vex_core_bytes) {

        const ee_bool_t requires_rex_r = bits & EE_PRV_X86_REX_R_MASK;
        const ee_bool_t requires_rex_x = bits & EE_PRV_X86_REX_X_MASK;
        const ee_bool_t requires_rex_b = bits & EE_PRV_X86_REX_B_MASK;
        const ee_bool_t requires_rex_w = bits & EE_PRV_X86_REX_W_MASK;

        if (ps->num_vex_core_bytes == 1) {

            const ee_bool_t is_rex_r_set = !(ps->vex_core_bytes[0] & EE_PRV_X86_VEX_R_MASK);

            return requires_rex_r && is_rex_r_set && !requires_rex_x && !requires_rex_b && !requires_rex_w;
        }
        else if (ps->num_vex_core_bytes == 2) {

            const ee_bool_t is_rex_r_set = !(ps->vex_core_bytes[0] & EE_PRV_X86_VEX_R_MASK);
            const ee_bool_t is_rex_x_set = !(ps->vex_core_bytes[0] & EE_PRV_X86_VEX_X_MASK);
            const ee_bool_t is_rex_b_set = !(ps->vex_core_bytes[0] & EE_PRV_X86_VEX_B_MASK);
            const ee_bool_t is_rex_w_set = ps->vex_core_bytes[1] & EE_PRV_X86_VEX_W_MASK;

            return (!requires_rex_r || is_rex_r_set)
                && (!requires_rex_x || is_rex_x_set)
                && (!requires_rex_b || is_rex_b_set)
                && (!requires_rex_w || is_rex_w_set);
        }
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_state_consume_modrm_sib(ee_prv_x86_state_t* distate, ee_bool_t requires_vsib, ee_prv_x86_modrm_resolved_effective_address_t* in_out_eff_addr) {

    const ee_prv_x86_modrm32_sib_mapping_t* const sib_mapping = ee_prv_x86_state_get_modrm32_sib_mapping(distate, requires_vsib);
    if (!sib_mapping)
        return EE_FALSE;

    if (sib_mapping->base) {

        /* Regular base scenario. */
        in_out_eff_addr->base_reg = sib_mapping->base;
    }
    else if (in_out_eff_addr->disp_bits) {

        /* MOD != 00B, EBP is implied as base. */
        in_out_eff_addr->base_reg = &EE_PRV_X86_MODRM_REG_PACK_EBP;
    }
    else {

        /* MOD == 00B, disp32 without base. */
        in_out_eff_addr->base_reg = 0;
        in_out_eff_addr->disp_bits = 32;
    }

    in_out_eff_addr->index_reg = sib_mapping->scaled_index;
    in_out_eff_addr->scaling_factor = sib_mapping->scaling;

    if (ee_prv_x86_prefix_state_are_rex_fields_set(&distate->active_prefixes, EE_PRV_X86_REX_X_MASK) && !in_out_eff_addr->index_reg)
        in_out_eff_addr->index_reg = &EE_PRV_X86_MODRM_REG_PACK_ESP;

    return EE_TRUE;
}

static ee_int64_t ee_prv_x86_apply_sign_extension(ee_int64_t in, ee_size_t in_size_bits) {

    ee_int64_t out = in;

    switch (in_size_bits) {

    case 8:

        if (in > EE_INT8_MAX)
            out = (ee_int8_t)in;
        break;

    case 16:

        if (in > EE_INT16_MAX)
            out = (ee_int16_t)in;
        break;

    case 32:

        if (in > EE_INT32_MAX)
            out = (ee_int32_t)in;
        break;
    }

    return out;
}

static ee_bool_t ee_prv_x86_state_consume_immediate_bytes(ee_prv_x86_state_t* distate, ee_size_t num_bits, ee_int64_t* immb) {

    const ee_size_t num_bytes = num_bits / 8;

    ee_int64_t le_immb = 0;
    ee_int64_t native_immb = 0;
    ee_byte_t bytes[sizeof(le_immb)] = { 0 };
    ee_size_t byte_index = 0;

    if (num_bits % 8)
        return EE_FALSE;

    if (!num_bytes)
        return EE_TRUE;

    if (num_bytes > EE_GET_ARRAY_LEN(bytes))
        return EE_FALSE;

    for (; byte_index != num_bytes; ++byte_index) {

        if (!ee_prv_x86_state_advance_bytes(distate, &bytes[byte_index]))
            return EE_FALSE;
    }

    ee_memcpy(&le_immb, bytes, sizeof(le_immb));

    native_immb = ee_endian_le_to_native_int64(le_immb);
    native_immb = ee_prv_x86_apply_sign_extension(native_immb, num_bits);

    *immb = native_immb;
    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_consume_displacement(ee_prv_x86_state_t* distate, ee_size_t num_bits, ee_int64_t* disp) {

    return ee_prv_x86_state_consume_immediate_bytes(distate, num_bits, disp);
}

static ee_bool_t ee_prv_x86_state_consume_modrm_byte(ee_prv_x86_state_t* distate, ee_bool_t requires_vsib, ee_prv_x86_modrm_resolved_effective_address_t* out_eff_addr, const ee_prv_x86_modrm_reg_pack_t** out_reg) {

    const ee_prv_x86_modrm_mapping_t* const modrm_mapping = ee_prv_x86_state_get_modrm_mapping(distate);
    ee_bool_t sib_consumed = EE_FALSE;

    if (!modrm_mapping)
        return EE_FALSE;

    if (modrm_mapping->eff_addr.type == EE_PRV_X86_EFFADDR_PTR) {

        /* Process pointer as effective address.
        */

        const ee_prv_x86_modrm_ptr_t* const ptr = &modrm_mapping->eff_addr.ptr;
        const ee_prv_x86_modrm_reg_pair_t* const base_regs = ptr->base;

        out_eff_addr->is_pointer = EE_TRUE;

        /* May be updated by SIB processing logic. */
        out_eff_addr->disp_bits = ptr->disp_bits;

        if (!base_regs) {

            /* Displacement without base registers (address).
            */

            const ee_bool_t mode_64 = distate->active_mode == EE_X86_MODE_64;
            out_eff_addr->base_reg = 0;
            out_eff_addr->index_reg = 0;

            if (mode_64)
                out_eff_addr->uses_rip_relative_addressing = EE_TRUE;
        }
        else if (!base_regs->reg0 && !base_regs->reg1) {

            /* SIB scenario.
            */

            if (!ee_prv_x86_state_consume_modrm_sib(distate, requires_vsib, out_eff_addr))
                return EE_FALSE;

            sib_consumed = EE_TRUE;
        }
        else {

            /* Regular base registers.
            */

            out_eff_addr->base_reg = base_regs->reg0;
            out_eff_addr->index_reg = base_regs->reg1;
        }

        if (out_eff_addr->disp_bits && !ee_prv_x86_state_consume_displacement(distate, out_eff_addr->disp_bits, &out_eff_addr->disp))
            return EE_FALSE;
    }
    else if (modrm_mapping->eff_addr.type == EE_PRV_X86_EFFADDR_REG) {

        /* Process raw register as effective address.
        */

        const ee_prv_x86_modrm_reg_pack_t* const reg = modrm_mapping->eff_addr.reg;
        if (!reg)
            return EE_FALSE;

        out_eff_addr->is_pointer = EE_FALSE;
        out_eff_addr->base_reg = reg;
    }
    else {

        return EE_FALSE;
    }

    if (requires_vsib && !sib_consumed)
        return EE_FALSE;

    *out_reg = modrm_mapping->reg;
    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_consume_plusi_register_code(const ee_prv_x86_state_t* distate, ee_byte_t* reg_code) {

    ee_size_t last_opcode_index = 0;
    ee_byte_t last_opcode_byte = 0;

    if (!distate->byte_index)
        return EE_FALSE;

    last_opcode_index = distate->byte_index - 1;
    if (last_opcode_index >= distate->num_bytes)
        return EE_FALSE;

    last_opcode_byte = distate->bytes[last_opcode_index];
    *reg_code = last_opcode_byte - distate->operand_base_byte;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_consume_plusr_register_code(const ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t** reg_pack) {

    ee_size_t last_opcode_index = 0;
    ee_byte_t last_opcode_byte = 0;
    ee_byte_t register_code = 0;

    if (!distate->byte_index)
        return EE_FALSE;

    last_opcode_index = distate->byte_index - 1;
    if (last_opcode_index >= distate->num_bytes)
        return EE_FALSE;

    last_opcode_byte = distate->bytes[last_opcode_index];
    register_code = last_opcode_byte & 7;

    if (register_code >= EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM_REG_PACKS))
        return EE_FALSE;

    *reg_pack = EE_PRV_X86_MODRM_REG_PACKS[register_code];
    return EE_TRUE;
}

static ee_x86_segment_register_type_t ee_prv_x86_prefix_state_to_segment_register(const ee_prv_x86_prefix_state_t* ps) {

    const ee_byte_t segreg_prefix_byte = ps->grp_prefixes[1];

    switch (segreg_prefix_byte) {

    case EE_PRV_X86_GRP2_PREFIX_ES_OVR:

        return EE_X86_SEGMENT_REGISTER_ES;

    case EE_PRV_X86_GRP2_PREFIX_CS_OVR:

        return EE_X86_SEGMENT_REGISTER_CS;

    case EE_PRV_X86_GRP2_PREFIX_SS_OVR:

        return EE_X86_SEGMENT_REGISTER_SS;

    case EE_PRV_X86_GRP2_PREFIX_DS_OVR:

        return EE_X86_SEGMENT_REGISTER_DS;

    case EE_PRV_X86_GRP2_PREFIX_FS_OVR:

        return EE_X86_SEGMENT_REGISTER_FS;

    case EE_PRV_X86_GRP2_PREFIX_GS_OVR:

        return EE_X86_SEGMENT_REGISTER_GS;
    }

    return EE_X86_SEGMENT_REGISTER_NOT_EXISTING;
}

static ee_x86_gpp_register_type_t ee_prv_x86_modrm_reg_pack_to_gpp_register(const ee_prv_x86_modrm_reg_pack_t* rp, ee_size_t bits) {

    if (!rp)
        return EE_X86_GPP_REGISTER_NOT_EXISTING;

    switch (bits) {

    case 8:

        return rp->gpp8;

    case 16:

        return rp->gpp16;

    case 32:

        return rp->gpp32;

    case 64:

        return rp->gpp64;
    }

    return EE_X86_GPP_REGISTER_NOT_EXISTING;
}

static ee_x86_segment_register_type_t ee_prv_x86_modrm_reg_pack_to_segment_register(const ee_prv_x86_modrm_reg_pack_t* rp) {

    if (!rp)
        return EE_X86_SEGMENT_REGISTER_NOT_EXISTING;

    switch (rp->index) {
    case 0: return EE_X86_SEGMENT_REGISTER_ES;
    case 1: return EE_X86_SEGMENT_REGISTER_CS;
    case 2: return EE_X86_SEGMENT_REGISTER_SS;
    case 3: return EE_X86_SEGMENT_REGISTER_DS;
    case 4: return EE_X86_SEGMENT_REGISTER_FS;
    case 5: return EE_X86_SEGMENT_REGISTER_GS;
    }

    return EE_X86_SEGMENT_REGISTER_NOT_EXISTING;
}

static ee_bool_t ee_prv_x86_state_transform_modrm_ptr_base_reg_to_ptr_operand_reg(const ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t* ptr_reg, ee_x86_pointer_operand_base_register_t* out) {

    if (!ptr_reg) {

        out->is_set = EE_FALSE;
        return EE_TRUE;
    }

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        /* Must be regular base register, check REX.B. */
        const ee_bool_t is_extended = ee_prv_x86_prefix_state_are_rex_fields_set(&distate->active_prefixes, EE_PRV_X86_REX_B_MASK);
        const ee_size_t reg_bits = ee_prv_x86_state_has_active_address_size_override(distate) ? 32 : 64;

        if (is_extended) {

            out->is_extended = EE_TRUE;
            out->un.ex_register.index = (ee_uint8_t)(ptr_reg->index + 8);
            out->un.ex_register.size_bits = (ee_uint16_t)reg_bits;
        }
        else {

            out->is_extended = EE_FALSE;
            out->un.reg1ster = ee_prv_x86_modrm_reg_pack_to_gpp_register(ptr_reg, reg_bits);
        }
    }
    else {

        const ee_size_t reg_bits = ee_prv_x86_state_must_use_modrm16_table(distate) ? 16 : 32;

        out->is_extended = EE_FALSE;
        out->un.reg1ster = ee_prv_x86_modrm_reg_pack_to_gpp_register(ptr_reg, reg_bits);
    }

    out->is_set = EE_TRUE;
    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_transform_modrm_ptr_index_reg_to_ptr_operand_reg(const ee_prv_x86_state_t* distate, ee_size_t vsib_index_reg_size_bits, const ee_prv_x86_modrm_reg_pack_t* ptr_reg, ee_x86_pointer_operand_index_register_t* out) {

    ee_memset(out, 0, sizeof(*out));

    if (!ptr_reg)
        return EE_TRUE;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        /* Must be SIB index register, check REX.X. */
        const ee_bool_t is_extended = ee_prv_x86_prefix_state_are_rex_fields_set(&distate->active_prefixes, EE_PRV_X86_REX_X_MASK);
        if (vsib_index_reg_size_bits) {

            const ee_uint8_t index_extension = is_extended ? 8 : 0;

            out->is_vec = EE_TRUE;
            out->un.numbered_register.index = (ee_uint8_t)(ptr_reg->index + index_extension);
            out->un.numbered_register.size_bits = (ee_uint16_t)vsib_index_reg_size_bits;
        }
        else {

            const ee_size_t reg_bits = ee_prv_x86_state_has_active_address_size_override(distate) ? 32 : 64;
            if (is_extended) {

                out->is_gpp_ex = EE_TRUE;
                out->un.numbered_register.index = (ee_uint8_t)(ptr_reg->index + 8);
                out->un.numbered_register.size_bits = (ee_uint16_t)reg_bits;
            }
            else {

                out->is_gpp = EE_TRUE;
                out->un.general_purpose_register = ee_prv_x86_modrm_reg_pack_to_gpp_register(ptr_reg, reg_bits);
            }
        }
    }
    else {

        if (vsib_index_reg_size_bits) {

            if (ee_prv_x86_state_must_use_modrm16_table(distate))
                return EE_FALSE;

            out->is_vec = EE_TRUE;
            out->un.numbered_register.index = (ee_uint8_t)ptr_reg->index;
            out->un.numbered_register.size_bits = (ee_uint16_t)vsib_index_reg_size_bits;
        }
        else {

            const ee_size_t reg_bits = ee_prv_x86_state_must_use_modrm16_table(distate) ? 16 : 32;

            out->is_gpp = EE_TRUE;
            out->un.general_purpose_register = ee_prv_x86_modrm_reg_pack_to_gpp_register(ptr_reg, reg_bits);
        }
    }

    out->is_set = EE_TRUE;
    return EE_TRUE;
}

static ee_x86_gpp_register_type_t ee_prv_x86_convert_gpp8_reg_to_x64_counterpart(ee_x86_gpp_register_type_t in) {

    switch (in) {
    case EE_X86_GPP_REGISTER_AH: return EE_X86_GPP_REGISTER_SPL;
    case EE_X86_GPP_REGISTER_CH: return EE_X86_GPP_REGISTER_BPL;
    case EE_X86_GPP_REGISTER_DH: return EE_X86_GPP_REGISTER_SIL;
    case EE_X86_GPP_REGISTER_BH: return EE_X86_GPP_REGISTER_DIL;
    default: break;
    }

    return in;
}

static ee_bool_t ee_prv_x86_is_register_type_extendable_via_rex(ee_x86_register_type_t reg_type) {

    return reg_type == EE_X86_REGISTER_GENERAL_PURPOSE
        || reg_type == EE_X86_REGISTER_AVX_128
        || reg_type == EE_X86_REGISTER_AVX_256
        || reg_type == EE_X86_REGISTER_CONTROL
        || reg_type == EE_X86_REGISTER_DEBUG;
}

static ee_bool_t ee_prv_x86_state_transform_modrm_reg_to_reg_operand(const ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t* modrm_reg, ee_bool_t is_rm_field, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_register_operand_t* out) {

    const ee_bool_t mode_64 = ee_prv_x86_state_is_64_bit_mode_active(distate);

    if (!modrm_reg)
        return EE_FALSE;

    if (mode_64) {

        ee_bool_t is_extended_reg = EE_FALSE;

        if (is_rm_field)
            is_extended_reg = ee_prv_x86_prefix_state_are_rex_fields_set(&distate->active_prefixes, EE_PRV_X86_REX_B_MASK);
        else
            is_extended_reg = ee_prv_x86_prefix_state_are_rex_fields_set(&distate->active_prefixes, EE_PRV_X86_REX_R_MASK);

        if (is_extended_reg) {

            if (ee_prv_x86_is_register_type_extendable_via_rex(reg_type)) {

                out->type = (reg_type == EE_X86_REGISTER_GENERAL_PURPOSE) ? EE_X86_REGISTER_GENERAL_PURPOSE_EX : reg_type;
                out->un.numbered_register.index = (ee_uint8_t)modrm_reg->index + 8;
                out->un.numbered_register.size_bits = (ee_uint16_t)reg_size_bits;

                return EE_TRUE;
            }
            else if (reg_type == EE_X86_REGISTER_MPX) {

                return EE_FALSE;
            }
        }
    }

    out->type = reg_type;

    if (reg_type == EE_X86_REGISTER_GENERAL_PURPOSE) {

        out->un.general_purpose_register = ee_prv_x86_modrm_reg_pack_to_gpp_register(modrm_reg, reg_size_bits);

        if (mode_64 && reg_size_bits == 8 && (distate->active_prefixes.rex_prefix & EE_PRV_X86_REX_MIN)) {

            /* In x64 mode with active REX prefix (NOT REX.B & REX.R), only SPL, BPL, SIL and DIL can be accessed, instead of
               AH, CH, DH and BH. This is not relevant for VEX instructions, as far as I know.
               Take note that REX.B and REX.R being unset is an already established fact and must not be verified again. If
               these fields were set, this function would have already returned.
            */

            out->un.general_purpose_register = ee_prv_x86_convert_gpp8_reg_to_x64_counterpart(out->un.general_purpose_register);
        }
    }
    else if (reg_type == EE_X86_REGISTER_SEGMENT) {

        out->un.segment_register = ee_prv_x86_modrm_reg_pack_to_segment_register(modrm_reg);
    }
    else {

        out->un.numbered_register.index = (ee_uint8_t)modrm_reg->index;
        out->un.numbered_register.size_bits = (ee_uint16_t)reg_size_bits;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(ee_prv_x86_state_t* distate, ee_bool_t requires_vsib, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_operand_t* rm, ee_x86_operand_t* reg) {

    /* If "requires_vsib" is true, "rm_reg_type" should be ignored and the size in bits (128 or 256) of the vector-based index register must be obtained from "rm_reg_size_bits".
    */

    ee_prv_x86_modrm_resolved_effective_address_t modrm_effaddr = { 0 };
    const ee_prv_x86_modrm_reg_pack_t* modrm_reg = 0;

    if (!ee_prv_x86_state_consume_modrm_byte(distate, requires_vsib, &modrm_effaddr, &modrm_reg))
        return EE_FALSE;

    if (rm) {

        if (modrm_effaddr.is_pointer) {

            const ee_prv_x86_prefix_state_t* const active_prefixes = &distate->active_prefixes;
            ee_x86_pointer_operand_t* const out_rm_ptr_operand = &rm->un.pointer;

            rm->type = EE_X86_OPERAND_POINTER;

            out_rm_ptr_operand->uses_rip_relative_addressing = modrm_effaddr.uses_rip_relative_addressing;
            out_rm_ptr_operand->type = rm_ptr_type;
            out_rm_ptr_operand->segment_register = ee_prv_x86_prefix_state_to_segment_register(active_prefixes);
            out_rm_ptr_operand->scaling = modrm_effaddr.index_reg ? modrm_effaddr.scaling_factor : 0;
            out_rm_ptr_operand->displacement_num_bits = modrm_effaddr.disp_bits;
            out_rm_ptr_operand->displacement = modrm_effaddr.disp;

            if (!ee_prv_x86_state_transform_modrm_ptr_base_reg_to_ptr_operand_reg(distate, modrm_effaddr.base_reg, &out_rm_ptr_operand->base_register)
                || !ee_prv_x86_state_transform_modrm_ptr_index_reg_to_ptr_operand_reg(distate, (requires_vsib ? rm_reg_size_bits : 0), modrm_effaddr.index_reg, &out_rm_ptr_operand->index_register)) {

                return EE_FALSE;
            }
        }
        else {

            ee_x86_register_operand_t* const out_rm_reg_operand = &rm->un.reg1ster;
            rm->type = EE_X86_OPERAND_REGISTER;

            if (!ee_prv_x86_state_transform_modrm_reg_to_reg_operand(distate, modrm_effaddr.base_reg, EE_TRUE, rm_reg_type, rm_reg_size_bits, out_rm_reg_operand))
                return EE_FALSE;
        }
    }

    if (reg) {

        reg->type = EE_X86_OPERAND_REGISTER;

        if (!ee_prv_x86_state_transform_modrm_reg_to_reg_operand(distate, modrm_reg, EE_FALSE, reg_type, reg_size_bits, &reg->un.reg1ster))
            return EE_FALSE;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_modrm_byte_into_rm_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr_type, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_operand_t* rm) {

    return ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(distate, EE_FALSE, ptr_type, reg_type, reg_size_bits, EE_X86_REGISTER_NOT_EXISTING, 0, rm, 0);
}

static ee_bool_t ee_prv_x86_state_process_immediate_bytes_into_operand(ee_prv_x86_state_t* distate, ee_size_t num_bits, ee_x86_operand_t* opd) {

    ee_int64_t immb = 0;

    if (!ee_prv_x86_state_consume_immediate_bytes(distate, num_bits, &immb))
        return EE_FALSE;

    opd->type = EE_X86_OPERAND_IMMEDIATE_VALUE;
    opd->un.immediate_value.value_num_bits = (ee_uint8_t)num_bits;
    opd->un.immediate_value.value = immb;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_relative_address_into_operand(ee_prv_x86_state_t* distate, ee_size_t num_bits, ee_x86_operand_t* opd) {

    ee_int64_t raw_offs = 0;

    /* rel64 cannot be handled by this function (it does not exist anyway). */
    if (num_bits > 32)
        return EE_FALSE;

    if (!ee_prv_x86_state_consume_immediate_bytes(distate, num_bits, &raw_offs))
        return EE_FALSE;

    opd->type = EE_X86_OPERAND_RELATIVE_ADDRESS;
    opd->un.relative_address.offset_num_bits = (ee_uint8_t)num_bits;
    opd->un.relative_address.offset = (ee_int32_t)raw_offs;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_vex_register_specifier_into_operand(const ee_prv_x86_state_t* distate, ee_uint8_t vex_reg_spec, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_operand_t* out) {

    ee_size_t actual_reg_index = 0;

    if (ee_prv_x86_state_is_64_bit_mode_active(distate))
        actual_reg_index = vex_reg_spec;
    else
        actual_reg_index = vex_reg_spec & 7;

    out->type = EE_X86_OPERAND_REGISTER;
    
    if (reg_type == EE_X86_REGISTER_AVX_128 || reg_type == EE_X86_REGISTER_AVX_256) {
        
        out->un.reg1ster.type = reg_type;
        out->un.reg1ster.un.numbered_register.index = (ee_uint8_t)actual_reg_index;
        out->un.reg1ster.un.numbered_register.size_bits = (ee_uint16_t)reg_size_bits;

        return EE_TRUE;
    }

    if (reg_type == EE_X86_REGISTER_GENERAL_PURPOSE) {

        const ee_prv_x86_modrm_reg_pack_t* reg_pack = 0;
        if (actual_reg_index < EE_GET_ARRAY_LEN(EE_PRV_X86_MODRM_REG_PACKS))
            reg_pack = EE_PRV_X86_MODRM_REG_PACKS[actual_reg_index];

        if (!reg_pack) {

            out->un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE_EX;
            out->un.reg1ster.un.numbered_register.index = (ee_uint8_t)actual_reg_index;
            out->un.reg1ster.un.numbered_register.size_bits = (ee_uint16_t)reg_size_bits;
        }
        else {

            out->un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE;
            
            if (reg_size_bits == 32) {
                out->un.reg1ster.un.general_purpose_register = reg_pack->gpp32;
            }
            else if (reg_size_bits == 64) {
                out->un.reg1ster.un.general_purpose_register = reg_pack->gpp64;
            }
            else {
                return EE_FALSE;
            }
        }

        return EE_TRUE;
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_state_process_modrm_reg_pack_into_operand(const ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t* reg_pack, ee_size_t reg_size_bits, ee_x86_operand_t* out) {

    out->type = EE_X86_OPERAND_REGISTER;

    return ee_prv_x86_state_transform_modrm_reg_to_reg_operand(distate, reg_pack, EE_TRUE, EE_X86_REGISTER_GENERAL_PURPOSE, reg_size_bits, &out->un.reg1ster);
}

static void ee_prv_x86_state_get_dynamic_modrm_operand_info(const ee_prv_x86_state_t* distate, ee_x86_pointer_type_t* ptr_type, ee_size_t* reg_size_bits) {

    ee_x86_pointer_type_t tmp_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_size_t tmp_reg_size_bits = 0;

    if (ee_prv_x86_state_has_active_rex_w(distate)) {

        tmp_ptr_type = EE_X86_POINTER_QWORD;
        tmp_reg_size_bits = 64;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {

        tmp_ptr_type = EE_X86_POINTER_WORD;
        tmp_reg_size_bits = 16;
    }
    else {

        tmp_ptr_type = EE_X86_POINTER_DWORD;
        tmp_reg_size_bits = 32;
    }

    *ptr_type = tmp_ptr_type;
    *reg_size_bits = tmp_reg_size_bits;
}

static ee_bool_t ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(const ee_prv_x86_state_t* distate, ee_uint8_t* vex_reg_spec, ee_x86_pointer_type_t* ptr_type, ee_x86_register_type_t* reg_type, ee_size_t* reg_size_bits) {

    ee_uint8_t vex_rxbwl = 0;

    if (!ee_prv_x86_state_dissect_active_vex_prefix(distate, &vex_rxbwl, vex_reg_spec))
        return EE_FALSE;

    if (!(vex_rxbwl & 1)) {

        *ptr_type = EE_X86_POINTER_XMMWORD;
        *reg_type = EE_X86_REGISTER_AVX_128;
        *reg_size_bits = 128;
    }
    else {

        *ptr_type = EE_X86_POINTER_YMMWORD;
        *reg_type = EE_X86_REGISTER_AVX_256;
        *reg_size_bits = 256;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_vex_get_register_specifier(const ee_prv_x86_state_t* distate, ee_uint8_t* reg_spec) {

    ee_x86_pointer_type_t ignored_ptr_type = EE_X86_POINTER_NOT_EXISTING;
    ee_x86_register_type_t ignored_reg_type = EE_X86_REGISTER_NOT_EXISTING;
    ee_size_t ignored_reg_size_bits = 0;

    return ee_prv_x86_state_vex_get_dynamic_modrm_operand_info(distate, reg_spec, &ignored_ptr_type, &ignored_reg_type, &ignored_reg_size_bits);
}

static ee_int32_t ee_prv_x86_state_advance_operand_index(ee_prv_x86_state_t* distate) {

    if (distate->num_operands >= EE_GET_ARRAY_LEN(distate->operands))
        return -1;

    return (ee_int32_t)distate->num_operands++;
}

static ee_bool_t ee_prv_x86_state_append_segreg_operand(ee_prv_x86_state_t* distate, ee_x86_segment_register_type_t reg) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_REGISTER;
    distate->operands[opd_index].un.reg1ster.type = EE_X86_REGISTER_SEGMENT;
    distate->operands[opd_index].un.reg1ster.un.segment_register = reg;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_append_gppreg_operand_as_is(ee_prv_x86_state_t* distate, ee_x86_gpp_register_type_t reg) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_REGISTER;
    distate->operands[opd_index].un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE;
    distate->operands[opd_index].un.reg1ster.un.general_purpose_register = reg;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_append_gppreg_operand_with_prefix_adjustments(ee_prv_x86_state_t* distate, const ee_prv_x86_modrm_reg_pack_t* gppreg_pack) {

    const ee_bool_t has_active_rex_w = ee_prv_x86_state_has_active_rex_w(distate);
    ee_x86_gpp_register_type_t actual_reg = gppreg_pack->gpp32;

    if (has_active_rex_w) {
        actual_reg = gppreg_pack->gpp64;
    }
    else if (ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {
        actual_reg = gppreg_pack->gpp16;
    }

    return ee_prv_x86_state_append_gppreg_operand_as_is(distate, actual_reg);
}

static ee_bool_t ee_prv_x86_state_append_numreg_operand(ee_prv_x86_state_t* distate, ee_x86_register_type_t type, ee_size_t index, ee_size_t size_bits) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_REGISTER;
    distate->operands[opd_index].un.reg1ster.type = type;
    distate->operands[opd_index].un.reg1ster.un.numbered_register.index = (ee_uint8_t)index;
    distate->operands[opd_index].un.reg1ster.un.numbered_register.size_bits = (ee_uint16_t)size_bits;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_append_x87reg_operand(ee_prv_x86_state_t* distate, ee_size_t index) {

    return ee_prv_x86_state_append_numreg_operand(distate, EE_X86_REGISTER_X87, index, 80);
}

static ee_bool_t ee_prv_x86_state_append_pointer_operand_as_is(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr, ee_x86_segment_register_type_t seg_reg, ee_x86_gpp_register_type_t base_reg) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_POINTER;
    distate->operands[opd_index].un.pointer.type = ptr;
    distate->operands[opd_index].un.pointer.segment_register = seg_reg;
    distate->operands[opd_index].un.pointer.base_register.is_set = EE_TRUE;
    distate->operands[opd_index].un.pointer.base_register.un.reg1ster = base_reg;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_append_pointer_operand_with_prefix_adjustments(ee_prv_x86_state_t* distate, ee_bool_t allow_qword_ptr_promotion, ee_x86_pointer_type_t opt_ptr, ee_x86_segment_register_type_t opt_seg_reg, const ee_prv_x86_modrm_reg_pack_t* base_reg_pack) {

    const ee_bool_t mode_64 = ee_prv_x86_state_is_64_bit_mode_active(distate);
    const ee_bool_t override_addr_size = ee_prv_x86_state_has_active_address_size_override(distate);
    const ee_bool_t has_active_rex_w = ee_prv_x86_state_has_active_rex_w(distate);

    ee_x86_pointer_type_t actual_ptr = EE_X86_POINTER_DWORD;
    ee_x86_segment_register_type_t actual_seg_reg = EE_X86_SEGMENT_REGISTER_NOT_EXISTING;
    ee_x86_gpp_register_type_t actual_base_reg = base_reg_pack->gpp32;

    if (opt_ptr != EE_X86_POINTER_NOT_EXISTING) {
        actual_ptr = opt_ptr;
    }
    else if (!has_active_rex_w && ee_prv_x86_state_hints_at_16_bit_operand_usage(distate)) {
        actual_ptr = EE_X86_POINTER_WORD;
    }
    else if (allow_qword_ptr_promotion && has_active_rex_w) {
        actual_ptr = EE_X86_POINTER_QWORD;
    }

    if (opt_seg_reg != EE_X86_SEGMENT_REGISTER_NOT_EXISTING) {

        if (!mode_64)
            actual_seg_reg = opt_seg_reg;
    }
    else {

        actual_seg_reg = ee_prv_x86_prefix_state_to_segment_register(&distate->active_prefixes);
    }

    if (!mode_64) {

        if (ee_prv_x86_state_hints_at_16_bit_address_usage(distate))
            actual_base_reg = base_reg_pack->gpp16;
    }
    else if (!override_addr_size) {

        actual_base_reg = base_reg_pack->gpp64;
    }

    return ee_prv_x86_state_append_pointer_operand_as_is(distate, actual_ptr, actual_seg_reg, actual_base_reg);
}

static ee_bool_t ee_prv_x86_state_append_far_pointer_operand(ee_prv_x86_state_t* distate, ee_uint16_t selector, ee_uint32_t offset, ee_size_t offset_size_bits) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_FAR_POINTER;
    distate->operands[opd_index].un.far_pointer.cs_selector = selector;
    distate->operands[opd_index].un.far_pointer.cs_offset_num_bits = (ee_uint8_t)offset_size_bits;
    distate->operands[opd_index].un.far_pointer.cs_offset = offset;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_append_imm_bytes_operand(ee_prv_x86_state_t* distate, ee_size_t imm_num_bits, ee_int64_t imm_value) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    distate->operands[opd_index].type = EE_X86_OPERAND_IMMEDIATE_VALUE;
    distate->operands[opd_index].un.immediate_value.value_num_bits = (ee_uint8_t)imm_num_bits;
    distate->operands[opd_index].un.immediate_value.value = imm_value;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_and_append_imm_bytes_operand(ee_prv_x86_state_t* distate, ee_size_t imm_bits) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    return ee_prv_x86_state_process_immediate_bytes_into_operand(distate, imm_bits, &distate->operands[opd_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_is4_numreg_operand(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits) {

    const ee_bool_t mode_64 = distate->active_mode == EE_X86_MODE_64;
    ee_int64_t raw_is4 = 0;
    ee_uint8_t is4 = 0;
    ee_uint64_t reg_index = 0;

    if (!ee_prv_x86_state_consume_immediate_bytes(distate, 8, &raw_is4))
        return EE_FALSE;

    is4 = (ee_uint8_t)raw_is4;
    is4 >>= 4;

    if (mode_64)
        reg_index = is4 & 15;
    else
        reg_index = is4 & 7;

    return ee_prv_x86_state_append_numreg_operand(distate, reg_type, (ee_size_t)reg_index, reg_size_bits);
}

static ee_bool_t ee_prv_x86_state_process_and_append_rel_address_operand(ee_prv_x86_state_t* distate, ee_size_t rel_addr_bits) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    return ee_prv_x86_state_process_relative_address_into_operand(distate, rel_addr_bits, &distate->operands[opd_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_rm_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    return ee_prv_x86_state_process_modrm_byte_into_rm_operand(distate, ptr, reg_type, reg_size_bits, &distate->operands[opd_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_m_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr) {

    return ee_prv_x86_state_process_and_append_modrm_rm_operand(distate, ptr, EE_X86_REGISTER_NOT_EXISTING, 0);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_bool_t reverse_order) {

    ee_int32_t reg_index = 0;
    ee_int32_t rm_index = 0;

    reg_index = ee_prv_x86_state_advance_operand_index(distate);
    if (reg_index < 0)
        return EE_FALSE;

    rm_index = ee_prv_x86_state_advance_operand_index(distate);
    if (rm_index < 0)
        return EE_FALSE;

    if (reverse_order) {

        const ee_int32_t old_reg_index = reg_index;

        reg_index = rm_index;
        rm_index = old_reg_index;
    }

    return ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(
        distate,
        EE_FALSE,
        rm_ptr_type,
        rm_reg_type,
        rm_reg_size_bits,
        reg_type,
        reg_size_bits,
        &distate->operands[rm_index],
        &distate->operands[reg_index]
    );
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_bool_t reverse_order) {

    return ee_prv_x86_state_process_and_append_modrm_reg_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        reg_type,
        reg_size_bits,
        reverse_order
    );
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec) {

    ee_int32_t vexVVVV_index = 0;
    ee_int32_t rm_index = 0;

    vexVVVV_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vexVVVV_index < 0)
        return EE_FALSE;

    rm_index = ee_prv_x86_state_advance_operand_index(distate);
    if (rm_index < 0)
        return EE_FALSE;

    return ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(distate, EE_FALSE, rm_ptr_type, rm_reg_type, rm_reg_size_bits, EE_X86_REGISTER_NOT_EXISTING, 0, &distate->operands[rm_index], 0)
        && ee_prv_x86_state_process_vex_register_specifier_into_operand(distate, vex_reg_spec, rm_reg_type, rm_reg_size_bits, &distate->operands[vexVVVV_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order) {

    ee_int32_t reg_index = 0;
    ee_int32_t vexVVVV_index = 0;
    ee_int32_t rm_index = 0;

    reg_index = ee_prv_x86_state_advance_operand_index(distate);
    if (reg_index < 0)
        return EE_FALSE;

    vexVVVV_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vexVVVV_index < 0)
        return EE_FALSE;

    rm_index = ee_prv_x86_state_advance_operand_index(distate);
    if (rm_index < 0)
        return EE_FALSE;

    if (reverse_reg_rm_order) {

        const ee_int32_t old_reg_index = reg_index;

        reg_index = rm_index;
        rm_index = old_reg_index;
    }

    return ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(distate, EE_FALSE, rm_ptr_type, rm_reg_type, rm_reg_size_bits, reg_type, reg_size_bits, &distate->operands[rm_index], &distate->operands[reg_index])
        && ee_prv_x86_state_process_vex_register_specifier_into_operand(distate, vex_reg_spec, reg_type, reg_size_bits, &distate->operands[vexVVVV_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_rm_vexVVVV_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_x86_register_type_t rm_reg_type, ee_size_t rm_reg_size_bits, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order) {

    ee_int32_t reg_index = 0;
    ee_int32_t rm_index = 0;
    ee_int32_t vexVVVV_index = 0;

    reg_index = ee_prv_x86_state_advance_operand_index(distate);
    if (reg_index < 0)
        return EE_FALSE;

    rm_index = ee_prv_x86_state_advance_operand_index(distate);
    if (rm_index < 0)
        return EE_FALSE;

    vexVVVV_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vexVVVV_index < 0)
        return EE_FALSE;

    if (reverse_reg_rm_order) {

        const ee_int32_t old_reg_index = reg_index;

        reg_index = rm_index;
        rm_index = old_reg_index;
    }

    return ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(distate, EE_FALSE, rm_ptr_type, rm_reg_type, rm_reg_size_bits, reg_type, reg_size_bits, &distate->operands[rm_index], &distate->operands[reg_index])
        && ee_prv_x86_state_process_vex_register_specifier_into_operand(distate, vex_reg_spec, reg_type, reg_size_bits, &distate->operands[vexVVVV_index]);
}

static ee_bool_t ee_prv_x86_state_verify_operands_vsib_conformity(const ee_prv_x86_state_t* distate) {

    const ee_x86_operand_t* opd0 = 0;
    const ee_x86_operand_t* opd1 = 0;
    const ee_x86_operand_t* opd2 = 0;

    const ee_x86_numbered_register_t* reg_opd = 0;
    const ee_x86_numbered_register_t* index_reg = 0;
    const ee_x86_numbered_register_t* vvvv_reg_opd = 0;

    if (distate->num_operands != 3)
        return EE_FALSE;

    opd0 = &distate->operands[0];
    opd1 = &distate->operands[1];
    opd2 = &distate->operands[2];

    if (opd0->type != EE_X86_OPERAND_REGISTER || opd1->type != EE_X86_OPERAND_POINTER || opd2->type != EE_X86_OPERAND_REGISTER)
        return EE_FALSE;

    if ((opd0->un.reg1ster.type != EE_X86_REGISTER_AVX_128 && opd0->un.reg1ster.type != EE_X86_REGISTER_AVX_256)
        || (opd2->un.reg1ster.type != EE_X86_REGISTER_AVX_128 && opd2->un.reg1ster.type != EE_X86_REGISTER_AVX_256)) {

        return EE_FALSE;
    }

    reg_opd = &opd0->un.reg1ster.un.numbered_register;
    vvvv_reg_opd = &opd2->un.reg1ster.un.numbered_register;

    if (!opd1->un.pointer.index_register.is_set)
        return EE_FALSE;

    if (!opd1->un.pointer.index_register.is_vec)
        return EE_FALSE;

    index_reg = &opd1->un.pointer.index_register.un.numbered_register;

    if (reg_opd->index == index_reg->index && reg_opd->size_bits == index_reg->size_bits)
        return EE_FALSE;

    if (index_reg->index == vvvv_reg_opd->index && index_reg->size_bits == vvvv_reg_opd->size_bits)
        return EE_FALSE;

    if (reg_opd->index == vvvv_reg_opd->index && reg_opd->size_bits == vvvv_reg_opd->size_bits)
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vsib_vexVVVV_operands_fine_grained(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t rm_ptr_type, ee_size_t vsib_index_reg_size_bits, ee_uint8_t vex_reg_spec) {

    ee_int32_t reg_index = 0;
    ee_int32_t vsib_index = 0;
    ee_int32_t vexVVVV_index = 0;

    reg_index = ee_prv_x86_state_advance_operand_index(distate);
    if (reg_index < 0)
        return EE_FALSE;

    vsib_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vsib_index < 0)
        return EE_FALSE;

    vexVVVV_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vexVVVV_index < 0)
        return EE_FALSE;

    if (!ee_prv_x86_state_process_modrm_byte_into_operands_fine_grained(distate, EE_TRUE, rm_ptr_type, EE_X86_REGISTER_NOT_EXISTING,
        vsib_index_reg_size_bits, reg_type, reg_size_bits, &distate->operands[vsib_index], &distate->operands[reg_index])) {
        
        return EE_FALSE;
    }

    if (!ee_prv_x86_state_process_vex_register_specifier_into_operand(distate, vex_reg_spec, reg_type, reg_size_bits, &distate->operands[vexVVVV_index]))
        return EE_FALSE;

    return ee_prv_x86_state_verify_operands_vsib_conformity(distate);
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_uint8_t vex_reg_spec, ee_bool_t reverse_reg_rm_order) {

    return ee_prv_x86_state_process_and_append_modrm_reg_vexVVVV_rm_operands_fine_grained(
        distate,
        reg_type,
        reg_size_bits,
        ptr_type,
        reg_type,
        reg_size_bits,
        vex_reg_spec,
        reverse_reg_rm_order
    );
}

static ee_bool_t ee_prv_x86_state_process_and_append_modrm_vexVVVV_rm_operands(ee_prv_x86_state_t* distate, ee_x86_register_type_t reg_type, ee_size_t reg_size_bits, ee_x86_pointer_type_t ptr_type, ee_uint8_t vex_reg_spec, ee_bool_t reverse_vexVVVV_rm_order) {

    ee_int32_t vexVVVV_index = 0;
    ee_int32_t rm_index = 0;

    vexVVVV_index = ee_prv_x86_state_advance_operand_index(distate);
    if (vexVVVV_index < 0)
        return EE_FALSE;

    rm_index = ee_prv_x86_state_advance_operand_index(distate);
    if (rm_index < 0)
        return EE_FALSE;

    if (reverse_vexVVVV_rm_order) {

        const ee_int32_t old_vexVVVV_index = vexVVVV_index;

        vexVVVV_index = rm_index;
        rm_index = old_vexVVVV_index;
    }

    return ee_prv_x86_state_process_vex_register_specifier_into_operand(distate, vex_reg_spec, reg_type, reg_size_bits, &distate->operands[vexVVVV_index])
        && ee_prv_x86_state_process_modrm_byte_into_rm_operand(distate, ptr_type, reg_type, reg_size_bits, &distate->operands[rm_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_plusi_x87reg_operand(ee_prv_x86_state_t* distate) {

    ee_byte_t reg_code = 0;
    if (!ee_prv_x86_state_consume_plusi_register_code(distate, &reg_code))
        return EE_FALSE;

    return ee_prv_x86_state_append_x87reg_operand(distate, reg_code);
}

static ee_bool_t ee_prv_x86_state_process_and_append_plusr_gppreg_operand(ee_prv_x86_state_t* distate, ee_size_t reg_size_bits) {

    const ee_prv_x86_modrm_reg_pack_t* reg_pack = 0;
    ee_int32_t opd_index = 0;

    if (!ee_prv_x86_state_consume_plusr_register_code(distate, &reg_pack))
        return EE_FALSE;

    opd_index = ee_prv_x86_state_advance_operand_index(distate);
    if (opd_index < 0)
        return EE_FALSE;

    return ee_prv_x86_state_process_modrm_reg_pack_into_operand(distate, reg_pack, reg_size_bits, &distate->operands[opd_index]);
}

static ee_bool_t ee_prv_x86_state_process_and_append_moffs_operand(ee_prv_x86_state_t* distate, ee_x86_pointer_type_t ptr_type) {

    const ee_int32_t opd_index = ee_prv_x86_state_advance_operand_index(distate);
    ee_size_t moffs_num_bits = 32;
    ee_x86_operand_t* opd = 0;

    if (opd_index < 0)
        return EE_FALSE;

    if (!ee_prv_x86_state_is_64_bit_mode_active(distate)) {

        if (ee_prv_x86_state_hints_at_16_bit_address_usage(distate))
            moffs_num_bits = 16;
    }
    else {

        if (!ee_prv_x86_state_has_active_address_size_override(distate))
            moffs_num_bits = 64;
    }

    opd = &distate->operands[opd_index];
    opd->type = EE_X86_OPERAND_POINTER;
    opd->un.pointer.type = ptr_type;
    opd->un.pointer.displacement_num_bits = (ee_uint8_t)moffs_num_bits;
    opd->un.pointer.segment_register = ee_prv_x86_prefix_state_to_segment_register(&distate->active_prefixes);

    return ee_prv_x86_state_consume_displacement(distate, moffs_num_bits, &opd->un.pointer.displacement);
}

static ee_bool_t ee_prv_x86_is_group_prefix_byte(ee_byte_t byte, ee_size_t* group_index) {

    if (byte < EE_PRV_X86_GRP2_PREFIX_ES_OVR || byte > EE_PRV_X86_GRP1_PREFIX_REP || !group_index)
        return EE_FALSE;

    switch (byte) {

    case EE_PRV_X86_GRP1_PREFIX_LOCK:
    case EE_PRV_X86_GRP1_PREFIX_REPNE:
    case EE_PRV_X86_GRP1_PREFIX_REP:

        *group_index = 0;
        return EE_TRUE;

    case EE_PRV_X86_GRP2_PREFIX_ES_OVR:
    case EE_PRV_X86_GRP2_PREFIX_CS_OVR:
    case EE_PRV_X86_GRP2_PREFIX_SS_OVR:
    case EE_PRV_X86_GRP2_PREFIX_DS_OVR:
    case EE_PRV_X86_GRP2_PREFIX_FS_OVR:
    case EE_PRV_X86_GRP2_PREFIX_GS_OVR:

        *group_index = 1;
        return EE_TRUE;

    case EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR:

        *group_index = 2;
        return EE_TRUE;

    case EE_PRV_X86_GRP4_PREFIX_ADR_SIZE_OVR:

        *group_index = 3;
        return EE_TRUE;
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_is_mandatory_group_prefix_candidate(ee_byte_t byte) {

    return byte == EE_PRV_X86_MANDATORY_GRP_PREFIX_66 || byte == EE_PRV_X86_MANDATORY_GRP_PREFIX_F2 || byte == EE_PRV_X86_MANDATORY_GRP_PREFIX_F3;
}

static ee_bool_t ee_prv_x86_collect_vex_prefix(ee_prv_x86_state_t* distate, ee_size_t vex_prefix_len) {

    const ee_size_t vex_beyond_end_index = distate->byte_index + vex_prefix_len;
    ee_byte_t cur_byte = 0;

    if (vex_beyond_end_index > distate->num_bytes)
        return EE_FALSE;

    /* Skip VEX intro byte. */
    if (!ee_prv_x86_state_advance_bytes(distate, &cur_byte))
        return EE_FALSE;

    while (distate->byte_index != vex_beyond_end_index && ee_prv_x86_state_advance_bytes(distate, &cur_byte)) {

        distate->active_prefixes.vex_core_bytes[distate->active_prefixes.num_vex_core_bytes++] = cur_byte;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_collect_prefixes(ee_prv_x86_state_t* distate) {

    /* Collects all prefix bytes and return EE_TRUE when the instruction byte has been reached. */
    do {

        const ee_bool_t mode_64 = distate->active_mode == EE_X86_MODE_64;
        ee_byte_t cur_byte = 0;
        ee_size_t group_index = 0;

        if (!ee_prv_x86_state_get_current_byte(distate, &cur_byte))
            return EE_FALSE;

        if (ee_prv_x86_is_group_prefix_byte(cur_byte, &group_index)) {

            /* Group prefixes from the same group override each other ("last one wins")...unless they are from group 1,
               in which case they stack (think about HEL and MPX). 
            */

            if (group_index == 0) {

                if (!distate->active_prefixes.grp_prefixes[group_index])
                    distate->active_prefixes.grp_prefixes[group_index] = cur_byte;
                else
                    distate->active_prefixes.stacked_grp1_prefix = cur_byte;
            }
            else {
                
                distate->active_prefixes.grp_prefixes[group_index] = cur_byte;
            }

            if (ee_prv_x86_is_mandatory_group_prefix_candidate(cur_byte))
                distate->active_prefixes.preliminary_mandatory_grp_prefix = cur_byte;

            /* Update final prefixes in a greedy fashion. If during opcode identification it turns out that a prefix has a
               mandatory nature, it will be unset again.
            */

            if (cur_byte == EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR)
                distate->final_prefixes.operand_size_override = EE_TRUE;
            else if (cur_byte == EE_PRV_X86_GRP4_PREFIX_ADR_SIZE_OVR)
                distate->final_prefixes.address_size_override = EE_TRUE;
        }
        else if (cur_byte >= EE_PRV_X86_REX_MIN && cur_byte <= EE_PRV_X86_REX_MAX) {

            ee_byte_t next_byte = 0;
            ee_size_t ignored_grp_index = 0;

            /* Check for INC / DEC instruction. */
            if (!mode_64)
                return EE_TRUE;

            /* Check for legacy prefix succeeding the REX prefix. */
            if (!ee_prv_x86_state_get_next_byte(distate, &next_byte))
                return EE_FALSE;

            /* Ignore REX prefix if the next byte is indeed a legacy prefix. */
            if (ee_prv_x86_is_group_prefix_byte(next_byte, &ignored_grp_index))
                continue;

            distate->active_prefixes.rex_prefix = cur_byte;

            /* No more prefix bytes after REX prefix. */
            return ee_prv_x86_state_advance_bytes(distate, &cur_byte);
        }
        else if (cur_byte == EE_PRV_X86_VEX2_INTRO || cur_byte == EE_PRV_X86_VEX3_INTRO) {

            const ee_size_t prefix_len_bytes = (cur_byte == EE_PRV_X86_VEX2_INTRO) ? 2 : 3;

            /* Check for LDS / LES instruction. */
            if (!mode_64) {

                ee_byte_t next_byte = 0;
                if (ee_prv_x86_state_get_next_byte(distate, &next_byte)) {

                    if ((next_byte & EE_PRV_X86_MODRM_MOD_MASK) != EE_PRV_X86_MODRM_MOD_MASK)
                        return EE_TRUE;
                }
            }

            /* No more prefix bytes after VEX prefix. */
            return ee_prv_x86_collect_vex_prefix(distate, prefix_len_bytes);
        }
        else {

            /* Instruction byte detected.  */
            return EE_TRUE;
        }

    } while (ee_prv_x86_state_just_advance_bytes(distate));

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86_collect_opcode_identity_list(ee_prv_x86_state_t* distate, const ee_prv_x86_opcode_identity_list_t** opcode_identity_list) {

    ee_byte_t cur_byte = 0;
    const ee_prv_x86_opcode_identity_list_t* identity_lists = 0;
    ee_size_t num_identity_lists = 0;

    if (!ee_prv_x86_state_get_current_byte(distate, &cur_byte))
        return EE_FALSE;

    if (!ee_prv_x86_state_has_active_vex_prefix(distate) && cur_byte == EE_PRV_X86_ESCAPE_OPCODE_0) {

        if (!ee_prv_x86_state_advance_and_get_current_byte(distate, &cur_byte))
            return EE_FALSE;

        if (cur_byte == EE_PRV_X86_ESCAPE_OPCODE_1_38) {

            if (!ee_prv_x86_state_advance_and_get_current_byte(distate, &cur_byte))
                return EE_FALSE;

            identity_lists = EE_PRV_X86_0F38d_OPCODE_IDENTITY_LISTS;
            num_identity_lists = EE_GET_ARRAY_LEN(EE_PRV_X86_0F38d_OPCODE_IDENTITY_LISTS);
        }
        else if (cur_byte == EE_PRV_X86_ESCAPE_OPCODE_1_3A) {

            if (!ee_prv_x86_state_advance_and_get_current_byte(distate, &cur_byte))
                return EE_FALSE;

            identity_lists = EE_PRV_X86_0F3Ad_OPCODE_IDENTITY_LISTS;
            num_identity_lists = EE_GET_ARRAY_LEN(EE_PRV_X86_0F3Ad_OPCODE_IDENTITY_LISTS);
        }
        else {

            identity_lists = EE_PRV_X86_0Fd_OPCODE_IDENTITY_LISTS;
            num_identity_lists = EE_GET_ARRAY_LEN(EE_PRV_X86_0Fd_OPCODE_IDENTITY_LISTS);
        }
    }
    else {

        identity_lists = EE_PRV_X86_OPCODE_IDENTITY_LISTS;
        num_identity_lists = EE_GET_ARRAY_LEN(EE_PRV_X86_OPCODE_IDENTITY_LISTS);
    }

    if (cur_byte >= num_identity_lists)
        return EE_FALSE;

    *opcode_identity_list = &identity_lists[cur_byte];

    return ee_prv_x86_state_just_advance_bytes(distate);
}

static ee_bool_t ee_prv_x86_check_mode_compatibility(ee_x86_mode_t active_mode, ee_prv_x86_supported_mode_t supported_mode) {

    /* At this point there is a distinction between three groups of instructions:
       
       1) Instructions available in every mode (SUPPMOD_32_64).
       2) Instructions available only in 32-bit mode (SUPPMOD_32).
       3) Instructions available only in 64-bit mode (SUPPMOD_64).
       
       If 16-bit OR 32-bit mode is active, instructions of groups 1) and 2) are considered being supported. If 64-bit mode is
       active, instructions of groups 1) and 3) are considered being supported.
    */

    const ee_bool_t mode_64 = active_mode == EE_X86_MODE_64;
    return supported_mode == EE_PRV_X86_SUPPMOD_32_64 || (mode_64 == (supported_mode == EE_PRV_X86_SUPPMOD_64));
}

static ee_int32_t ee_prv_x86_lookup_mandatory_prefix_group_index(ee_byte_t prefix_byte) {

    switch (prefix_byte) {

    case EE_PRV_X86_MANDATORY_GRP_PREFIX_F2:
    case EE_PRV_X86_MANDATORY_GRP_PREFIX_F3:

        /* Prefixes F2h and F3h belong to group 1. */
        return 0;

    case EE_PRV_X86_MANDATORY_GRP_PREFIX_66:

        /* Prefix 66h belongs to group 3. */
        return 2;
    }

    return -1;
}

static ee_bool_t ee_prv_x86_check_grp_prefix_constraints(const ee_prv_x86_prefix_state_t* prefix_state, const ee_prv_x86_grp_prefix_constraints_t* constraints, ee_bool_t* secondary_constraints_met) {

    const ee_size_t num_set_prefixes = EE_GET_ARRAY_LEN(prefix_state->grp_prefixes);
    const ee_size_t num_prohibited_prefixes = EE_GET_ARRAY_LEN(constraints->prohibited_prefixes);
    ee_size_t prohibited_prefix_index = 0;
    ee_bool_t mandatory_prefix_set = EE_FALSE;
    ee_bool_t mandatory_prefix_available = EE_FALSE;
    ee_bool_t prohibited_prefixes_unset = EE_TRUE;

    *secondary_constraints_met = EE_FALSE;

    if (constraints->mandatory_prefix == EE_PRV_X86_CONSTRAINT_EMPTY) {

        mandatory_prefix_set = EE_TRUE;
    }
    else {

        const ee_byte_t mandatory_prefix_byte = (ee_byte_t)constraints->mandatory_prefix;
        if (prefix_state->preliminary_mandatory_grp_prefix == mandatory_prefix_byte) {

            mandatory_prefix_set = EE_TRUE;
        }
        else {

            const ee_int32_t group_index = ee_prv_x86_lookup_mandatory_prefix_group_index((ee_byte_t)mandatory_prefix_byte);
            if (group_index >= 0 && (ee_size_t)group_index < num_set_prefixes) {

                if (prefix_state->grp_prefixes[group_index] == mandatory_prefix_byte)
                    mandatory_prefix_available = EE_TRUE;

                if (!group_index) {

                    /* Special case for group 1: Check stacked prefix byte. */
                    if(prefix_state->stacked_grp1_prefix == mandatory_prefix_byte)
                        mandatory_prefix_available = EE_TRUE;
                }
            }
        }
    }

    for (; prohibited_prefix_index != num_prohibited_prefixes; ++prohibited_prefix_index) {

        const ee_int16_t prohibited_prefix_byte = constraints->prohibited_prefixes[prohibited_prefix_index];
        ee_int32_t group_index = 0;

        if (prohibited_prefix_byte == EE_PRV_X86_CONSTRAINT_EMPTY)
            break;

        group_index = ee_prv_x86_lookup_mandatory_prefix_group_index((ee_byte_t)prohibited_prefix_byte);
        if (group_index < 0 || (ee_size_t)group_index >= num_set_prefixes)
            return EE_FALSE;

        if (prefix_state->grp_prefixes[group_index] == prohibited_prefix_byte) {

            prohibited_prefixes_unset = EE_FALSE;
            break;
        }

        if (!group_index) {

            /* Special case for group 1: Check stacked prefix byte. */
            if (prefix_state->stacked_grp1_prefix == prohibited_prefix_byte) {
                
                prohibited_prefixes_unset = EE_FALSE;
                break;
            }
        }
    }

    if (mandatory_prefix_available && prohibited_prefixes_unset)
        *secondary_constraints_met = EE_TRUE;

    return mandatory_prefix_set && prohibited_prefixes_unset;
}

static ee_bool_t ee_prv_x86_check_grp_rex_prefix_constraints(const ee_prv_x86_state_t* distate, const ee_prv_x86_grp_rex_prefix_constraints_t* constraints, ee_bool_t* secondary_constraints_met) {

    const ee_bool_t mode_64 = ee_prv_x86_state_is_64_bit_mode_active(distate);

    *secondary_constraints_met = EE_FALSE;

    if (constraints->mandatory_rex_bits != EE_PRV_X86_CONSTRAINT_EMPTY) {

        const ee_byte_t bit_mask = (ee_byte_t)constraints->mandatory_rex_bits;
        if (!mode_64)
            return EE_FALSE;

        if ((distate->active_prefixes.rex_prefix & bit_mask) != bit_mask)
            return EE_FALSE;
    }

    if (mode_64 && constraints->prohibited_rex_bits != EE_PRV_X86_CONSTRAINT_EMPTY) {

        const ee_byte_t bit_mask = (ee_byte_t)constraints->prohibited_rex_bits;
        if (distate->active_prefixes.rex_prefix & bit_mask)
            return EE_FALSE;
    }

    return ee_prv_x86_check_grp_prefix_constraints(&distate->active_prefixes, &constraints->grp_cons, secondary_constraints_met);
}

static ee_bool_t ee_prv_x86_check_vex_prefix_constraints(const ee_prv_x86_state_t* distate, const ee_prv_x86_vex_prefix_constraints_t* constraints) {

    const ee_prv_x86_prefix_state_t* const prefix_state = &distate->active_prefixes;

    ee_byte_t set_vex_mmmmm = 0;
    ee_byte_t set_vex_w = 0;
    ee_byte_t set_vex_l = 0;
    ee_byte_t set_vex_pp = 0;
    ee_byte_t set_vex_vvvv_raw = 0;
    ee_int16_t mandatory_w = EE_PRV_X86_CONSTRAINT_EMPTY;

    if (!prefix_state->num_vex_core_bytes)
        return EE_FALSE;

    if (prefix_state->num_vex_core_bytes == 1) {

        set_vex_mmmmm = EE_PRV_X86_VEX_MMMMM_0F;
        set_vex_l = (prefix_state->vex_core_bytes[0] & EE_PRV_X86_VEX_L_MASK) >> 2;
        set_vex_pp = prefix_state->vex_core_bytes[0] & EE_PRV_X86_VEX_PP_MASK;
        set_vex_vvvv_raw = (prefix_state->vex_core_bytes[0] & EE_PRV_X86_VEX_VVVV_MASK) >> 3;
    }
    else if (prefix_state->num_vex_core_bytes == 2) {

        set_vex_mmmmm = prefix_state->vex_core_bytes[0] & EE_PRV_X86_VEX_MMMMM_MASK;
        set_vex_w = (prefix_state->vex_core_bytes[1] & EE_PRV_X86_VEX_W_MASK) >> 7;
        set_vex_l = (prefix_state->vex_core_bytes[1] & EE_PRV_X86_VEX_L_MASK) >> 2;
        set_vex_pp = prefix_state->vex_core_bytes[1] & EE_PRV_X86_VEX_PP_MASK;
        set_vex_vvvv_raw = (prefix_state->vex_core_bytes[1] & EE_PRV_X86_VEX_VVVV_MASK) >> 3;
    }

    if (constraints->mandatory_l != EE_PRV_X86_CONSTRAINT_EMPTY && set_vex_l != constraints->mandatory_l)
        return EE_FALSE;

    if (constraints->mandatory_pp != EE_PRV_X86_CONSTRAINT_EMPTY && set_vex_pp != constraints->mandatory_pp)
        return EE_FALSE;

    if (constraints->mandatory_mmmmm != EE_PRV_X86_CONSTRAINT_EMPTY && set_vex_mmmmm != constraints->mandatory_mmmmm)
        return EE_FALSE;

    if (!ee_prv_x86_state_is_64_bit_mode_active(distate))
        mandatory_w = constraints->mandatory_w_x86;
    else
        mandatory_w = constraints->mandatory_w_x64;

    if (mandatory_w != EE_PRV_X86_CONSTRAINT_EMPTY && set_vex_w != mandatory_w)
        return EE_FALSE;

    if (constraints->mandatory_raw_vvvv != EE_PRV_X86_CONSTRAINT_EMPTY && set_vex_vvvv_raw != constraints->mandatory_raw_vvvv)
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_check_address_size_override_prefix_constraints(const ee_prv_x86_state_t* distate, const ee_prv_x86_address_size_override_prefix_constraints_t* constraints) {

    const ee_bool_t mode_64 = ee_prv_x86_state_is_64_bit_mode_active(distate);
    const ee_bool_t has_aso_prefix = ee_prv_x86_state_has_active_address_size_override(distate);

    if (!mode_64) {

        if (constraints->mandatory_for_x86 && !has_aso_prefix)
            return EE_FALSE;

        if (constraints->prohibited_for_x86 && has_aso_prefix)
            return EE_FALSE;
    }
    else {

        if (constraints->mandatory_for_x64 && !has_aso_prefix)
            return EE_FALSE;

        if (constraints->prohibited_for_x64 && has_aso_prefix)
            return EE_FALSE;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_check_prefix_compatibility(const ee_prv_x86_state_t* distate, const ee_prv_x86_opcode_identity_t* opcode_ident, ee_bool_t* secondary_compatible) {

    const ee_prv_x86_prefix_state_t* const prefix_state = &distate->active_prefixes;
    const ee_prv_x86_prefix_constraints_t* const prefix_constraints = opcode_ident->prefix_constraints;
    ee_bool_t check_has_passed = EE_TRUE;

    *secondary_compatible = EE_FALSE;

    /* Check for active VEX mode, in which case the opcode identity must have mandatory prefix bits of type VEX. */
    if (prefix_state->num_vex_core_bytes && (!prefix_constraints || (prefix_constraints->family != EE_PRV_X86_PREFAM_VEX)))
        return EE_FALSE;

    if (prefix_constraints) {

        switch (prefix_constraints->family) {

        case EE_PRV_X86_PREFAM_GRP:

            check_has_passed = ee_prv_x86_check_grp_prefix_constraints(prefix_state, (const ee_prv_x86_grp_prefix_constraints_t*)prefix_constraints, secondary_compatible);
            break;

        case EE_PRV_X86_PREFAM_GRP_REX:

            check_has_passed = ee_prv_x86_check_grp_rex_prefix_constraints(distate, (const ee_prv_x86_grp_rex_prefix_constraints_t*)prefix_constraints, secondary_compatible);
            break;

        case EE_PRV_X86_PREFAM_VEX:

            check_has_passed = ee_prv_x86_check_vex_prefix_constraints(distate, (const ee_prv_x86_vex_prefix_constraints_t*)prefix_constraints);
            break;

        case EE_PRV_X86_PREFAM_ADDRESS_SIZE_OVERRIDE:

            check_has_passed = ee_prv_x86_check_address_size_override_prefix_constraints(distate, (const ee_prv_x86_address_size_override_prefix_constraints_t*)prefix_constraints);
            break;

        default:

            break;
        }
    }

    return check_has_passed;
}

static ee_bool_t ee_prv_x86_check_modrm_operand_constraints(ee_byte_t operand_byte, const ee_prv_x86_modrm_operand_constraints_t* constraints) {

    const ee_byte_t set_modrm_mod = (operand_byte & EE_PRV_X86_MODRM_MOD_MASK) >> 6;
    const ee_byte_t set_modrm_rm = (operand_byte & EE_PRV_X86_MODRM_RM_MASK);
    const ee_byte_t set_modrm_reg = (operand_byte & EE_PRV_X86_MODRM_REG_MASK) >> 3;

    if (constraints->mandatory_mod != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_mod != constraints->mandatory_mod)
        return EE_FALSE;

    if (constraints->mandatory_rm != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_rm != constraints->mandatory_rm)
        return EE_FALSE;

    if (constraints->mandatory_reg != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_reg != constraints->mandatory_reg)
        return EE_FALSE;

    if (constraints->prohibited_mod != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_mod == constraints->prohibited_mod)
        return EE_FALSE;

    if (constraints->prohibited_rm != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_rm == constraints->prohibited_rm)
        return EE_FALSE;

    if (constraints->prohibited_reg != EE_PRV_X86_CONSTRAINT_EMPTY && set_modrm_reg == constraints->prohibited_reg)
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_check_modrm_operand_bit_constraints(ee_byte_t operand_byte, const ee_prv_x86_modrm_operand_bit_constraints_t* constraints) {

    const ee_byte_t set_modrm_mod = (operand_byte & EE_PRV_X86_MODRM_MOD_MASK) >> 6;
    const ee_byte_t set_modrm_rm = (operand_byte & EE_PRV_X86_MODRM_RM_MASK);
    const ee_byte_t set_modrm_reg = (operand_byte & EE_PRV_X86_MODRM_REG_MASK) >> 3;

    if (constraints->mandatory_mod_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_mod & constraints->mandatory_mod_bit_comb) != constraints->mandatory_mod_bit_comb)
        return EE_FALSE;

    if (constraints->mandatory_rm_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_rm & constraints->mandatory_rm_bit_comb) != constraints->mandatory_rm_bit_comb)
        return EE_FALSE;

    if (constraints->mandatory_reg_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_reg & constraints->mandatory_reg_bit_comb) != constraints->mandatory_reg_bit_comb)
        return EE_FALSE;

    if (constraints->prohibited_mod_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_mod & constraints->prohibited_mod_bit_comb) == constraints->prohibited_mod_bit_comb)
        return EE_FALSE;

    if (constraints->prohibited_rm_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_rm & constraints->prohibited_rm_bit_comb) == constraints->prohibited_rm_bit_comb)
        return EE_FALSE;

    if (constraints->prohibited_reg_bit_comb != EE_PRV_X86_CONSTRAINT_EMPTY && (set_modrm_reg & constraints->prohibited_reg_bit_comb) == constraints->prohibited_reg_bit_comb)
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_check_byte_range_operand_constraints(ee_byte_t operand_byte, const ee_prv_x86_byte_range_operand_constraints_t* constraints) {

    if (constraints->first && !constraints->last)
        return operand_byte == constraints->first;

    return operand_byte >= constraints->first && operand_byte <= constraints->last;
}

static ee_bool_t ee_prv_x86_check_operand_compatibility(const ee_prv_x86_state_t* distate, const ee_prv_x86_opcode_identity_t* opcode_ident) {

    const ee_prv_x86_operand_constraints_t* const operand_constraints = opcode_ident->operand_constraints;
    ee_byte_t operand_byte = 0;
    ee_bool_t check_has_passed = EE_TRUE;

    if (!operand_constraints)
        return EE_TRUE;

    if (!ee_prv_x86_state_get_current_byte(distate, &operand_byte))
        return EE_FALSE;

    switch (operand_constraints->family) {

    case EE_PRV_X86_OPDFAM_MODRM:

        check_has_passed = ee_prv_x86_check_modrm_operand_constraints(operand_byte, (const ee_prv_x86_modrm_operand_constraints_t*)operand_constraints);
        break;

    case EE_PRV_X86_OPDFAM_MODRM_BITS:

        check_has_passed = ee_prv_x86_check_modrm_operand_bit_constraints(operand_byte, (const ee_prv_x86_modrm_operand_bit_constraints_t*)operand_constraints);
        break;

    case EE_PRV_X86_OPDFAM_BYTE_RANGE:

        check_has_passed = ee_prv_x86_check_byte_range_operand_constraints(operand_byte, (const ee_prv_x86_byte_range_operand_constraints_t*)operand_constraints);
        break;

    default:

        break;
    }

    return check_has_passed;
}

static void ee_prv_x86_state_confirm_mandatory_prefix_byte(ee_prv_x86_state_t* distate, const ee_prv_x86_opcode_identity_t* opcode_ident) {

    if (!opcode_ident->prefix_constraints)
        return;

    if (opcode_ident->prefix_constraints->family == EE_PRV_X86_PREFAM_GRP
        || opcode_ident->prefix_constraints->family == EE_PRV_X86_PREFAM_GRP_REX) {

        const ee_prv_x86_grp_prefix_constraints_t* constraints = 0;
        if (opcode_ident->prefix_constraints->family == EE_PRV_X86_PREFAM_GRP) {
            
            constraints = (const ee_prv_x86_grp_prefix_constraints_t*)opcode_ident->prefix_constraints;
        }
        else if (opcode_ident->prefix_constraints->family == EE_PRV_X86_PREFAM_GRP_REX) {

            const ee_prv_x86_grp_rex_prefix_constraints_t* wrapped_constraints = (const ee_prv_x86_grp_rex_prefix_constraints_t*)opcode_ident->prefix_constraints;
            constraints = &wrapped_constraints->grp_cons;
        }

        if (constraints->mandatory_prefix == EE_PRV_X86_CONSTRAINT_EMPTY)
            return;

        distate->active_prefixes.confirmed_mandatory_grp_prefix = (ee_byte_t)constraints->mandatory_prefix;

        /* Ensure operand size override is EE_FALSE if the prefix is mandatory. */
        if ((ee_byte_t)constraints->mandatory_prefix == EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR)
            distate->final_prefixes.operand_size_override = EE_FALSE;

        return;
    }
}

static ee_uint32_t ee_prv_x86_is_opcode_identity_eligible_for_hle(const ee_prv_x86_opcode_identity_t* opcode_ident) {

    if (opcode_ident->flags & EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_LOCK)
        return EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_LOCK;

    if (opcode_ident->flags & EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WIWO_LOCK)
        return EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WIWO_LOCK;

    if (opcode_ident->flags & EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_WIWO_LOCK)
        return EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_WIWO_LOCK;

    return 0;
}

static ee_bool_t ee_prv_x86_identify_opcode(ee_prv_x86_state_t* distate, const ee_prv_x86_opcode_identity_list_t* opcode_identity_list) {

    ee_size_t ident_index = 0;
    const ee_prv_x86_opcode_identity_t* ident = 0;
    const ee_prv_x86_opcode_identity_t* secondary_ident = 0;

    if (!opcode_identity_list->elements || !opcode_identity_list->num_elements)
        return EE_FALSE;

    for (; !ident && ident_index != opcode_identity_list->num_elements; ++ident_index) {

        const ee_prv_x86_opcode_identity_t* const cur_ident = &opcode_identity_list->elements[ident_index];
        if (ee_prv_x86_check_mode_compatibility(distate->active_mode, cur_ident->supported_mode)
            && ee_prv_x86_check_operand_compatibility(distate, cur_ident)) {

            ee_bool_t secondary_compatible = EE_FALSE;
            if (ee_prv_x86_check_prefix_compatibility(distate, cur_ident, &secondary_compatible)) {
                ident = cur_ident;
            }
            else if (secondary_compatible) {
                secondary_ident = cur_ident;
            }
        }
    }

    if (!ident && secondary_ident)
        ident = secondary_ident;

    if (ident) {

        ee_prv_x86_state_confirm_mandatory_prefix_byte(distate, ident);

        distate->instruction = ident->instruction;
        distate->process_operand_bytes = ident->process_operand_bytes;

        /* Determine instruction flags. */
        distate->instruction_flags.is_eligible_for_branch_hint = ident->flags & EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_BRANCH_HINT;
        distate->instruction_flags.is_eligible_for_pseudonymization = ident->flags & EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_PSEUDONYMIZATION;
        distate->instruction_flags.is_eligible_for_hle = ee_prv_x86_is_opcode_identity_eligible_for_hle(ident);
        distate->instruction_flags.is_eligible_for_bnd_prefix = ident->flags & EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_BND_PREFIX;

        if (ident->operand_constraints && ident->operand_constraints->family == EE_PRV_X86_OPDFAM_BYTE_RANGE) {

            const ee_prv_x86_byte_range_operand_constraints_t* const br_constraints = (const ee_prv_x86_byte_range_operand_constraints_t*)ident->operand_constraints;

            /* Special case: The operand byte won't be consumed for instructions which have a byte selector or range as operand.
                             In order to obtain the correct instruction length, the state has to be advanced "manually" at this point.
            */
            if (!ee_prv_x86_state_just_advance_bytes(distate))
                return EE_FALSE;

            distate->operand_base_byte = br_constraints->first;
        }
    }

    return ident != 0;
}

static ee_x86_prefix_type_t ee_prv_x86_lookup_grp1_prefix_type(ee_byte_t pb, ee_bool_t is_xacquire_enabled, ee_bool_t is_xrelease_enabled, ee_bool_t is_bnd_enabled) {

    switch (pb) {

    case EE_PRV_X86_GRP1_PREFIX_LOCK:

        return EE_X86_PREFIX_LOCK;

    case EE_PRV_X86_GRP1_PREFIX_REPNE:

        if (is_xacquire_enabled)
            return EE_X86_PREFIX_XACQUIRE;

        if (is_bnd_enabled)
            return EE_X86_PREFIX_BND;
        
        return EE_X86_PREFIX_REPNE;

    case EE_PRV_X86_GRP1_PREFIX_REP:

        if (is_xrelease_enabled)
            return EE_X86_PREFIX_XRELEASE;

        return EE_X86_PREFIX_REP;
    }

    return EE_X86_PREFIX_NOT_EXISTING;
}

static ee_bool_t ee_prv_x86_state_has_memory_destination_operand(const ee_prv_x86_state_t* distate) {

    if (distate->num_operands > 0 && distate->operands[0].type == EE_X86_OPERAND_POINTER)
        return EE_TRUE;

    if (distate->num_operands > 1 && distate->instruction == EE_X86_INSTRUCTION_XCHG && distate->operands[1].type == EE_X86_OPERAND_POINTER)
        return EE_TRUE;

    return EE_FALSE;
}

static ee_size_t ee_prv_x86_identify_grp1_printable_prefixes(ee_prv_x86_state_t* distate) {

    const ee_prv_x86_prefix_state_t* const ps = &distate->active_prefixes;
    const ee_byte_t grp1_pb = distate->active_prefixes.grp_prefixes[0];
    const ee_byte_t stacked_grp1_pb = distate->active_prefixes.stacked_grp1_prefix;
    const ee_bool_t is_bnd_enabled = distate->instruction_flags.is_eligible_for_bnd_prefix;

    ee_byte_t primary_pb = 0;
    ee_byte_t secondary_pb = 0;
    ee_size_t prefix_index = 0;

    ee_bool_t is_xacquire_enabled = EE_FALSE;
    ee_bool_t is_xrelease_enabled = EE_FALSE;
    ee_x86_prefix_type_t prefix0 = EE_X86_PREFIX_NOT_EXISTING;
    ee_x86_prefix_type_t prefix1 = EE_X86_PREFIX_NOT_EXISTING;

    if (EE_GET_ARRAY_LEN(distate->final_prefixes.printable_prefixes) < 2)
        return 0;

    if (stacked_grp1_pb) {

        if (stacked_grp1_pb == ps->confirmed_mandatory_grp_prefix) {

            primary_pb = grp1_pb;
        }
        else {

            primary_pb = stacked_grp1_pb;
            secondary_pb = grp1_pb;
        }
    }
    else {

        if (grp1_pb != ps->confirmed_mandatory_grp_prefix)
            primary_pb = grp1_pb;
    }

    if (distate->instruction_flags.is_eligible_for_hle && ee_prv_x86_state_has_memory_destination_operand(distate)) {

        if (distate->instruction_flags.is_eligible_for_hle == EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_LOCK) {

            const ee_bool_t has_lock = primary_pb == EE_PRV_X86_GRP1_PREFIX_LOCK || secondary_pb == EE_PRV_X86_GRP1_PREFIX_LOCK;
            is_xacquire_enabled = is_xrelease_enabled = has_lock;
        }
        else if (distate->instruction_flags.is_eligible_for_hle == EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WIWO_LOCK) {

            is_xacquire_enabled = is_xrelease_enabled = EE_TRUE;
        }
        else if (distate->instruction_flags.is_eligible_for_hle == EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_WIWO_LOCK) {

            const ee_bool_t has_lock = primary_pb == EE_PRV_X86_GRP1_PREFIX_LOCK || secondary_pb == EE_PRV_X86_GRP1_PREFIX_LOCK;
            is_xacquire_enabled = has_lock;
            is_xrelease_enabled = EE_TRUE;
        }
    }

    if (primary_pb) {
        
        prefix0 = ee_prv_x86_lookup_grp1_prefix_type(primary_pb, is_xacquire_enabled, is_xrelease_enabled, is_bnd_enabled);
        if (prefix0 != EE_X86_PREFIX_NOT_EXISTING)
            distate->final_prefixes.printable_prefixes[prefix_index++] = prefix0;
    }

    if (secondary_pb) {
        
        prefix1 = ee_prv_x86_lookup_grp1_prefix_type(secondary_pb, is_xacquire_enabled, is_xrelease_enabled, is_bnd_enabled);
        if (prefix1 != EE_X86_PREFIX_NOT_EXISTING)
            distate->final_prefixes.printable_prefixes[prefix_index++] = prefix1;
    }

    return prefix_index;
}

static ee_size_t ee_prv_x86_identify_grp2_printable_prefixes(ee_prv_x86_state_t* distate, ee_size_t prefix_index) {

    if (prefix_index >= EE_GET_ARRAY_LEN(distate->final_prefixes.printable_prefixes))
        return prefix_index;

    if (distate->instruction_flags.is_eligible_for_branch_hint) {

        const ee_byte_t grp2_pb = distate->active_prefixes.grp_prefixes[1];
        switch (grp2_pb) {

        case EE_PRV_X86_GRP2_PREFIX_BRANCH_NOT_TAKEN:

            distate->final_prefixes.printable_prefixes[prefix_index++] = EE_X86_PREFIX_HNT;
            break;

        case EE_PRV_X86_GRP2_PREFIX_BRANCH_TAKEN:

            distate->final_prefixes.printable_prefixes[prefix_index++] = EE_X86_PREFIX_HT;
            break;
        }
    }

    return prefix_index;
}

static void ee_prv_x86_identify_printable_prefixes(ee_prv_x86_state_t* distate) {

    /* Must only be called AFTER operands have been collected.
    */

    ee_size_t prefix_index = 0;
    ee_memset(&distate->final_prefixes.printable_prefixes, 0, sizeof(distate->final_prefixes.printable_prefixes));

    prefix_index = ee_prv_x86_identify_grp1_printable_prefixes(distate);
    prefix_index = ee_prv_x86_identify_grp2_printable_prefixes(distate, prefix_index);

    distate->final_prefixes.num_printable_prefixes = prefix_index;
}

static ee_x86_instruction_type_t ee_prv_x86_lookup_instruction_pseudonym(ee_x86_instruction_type_t inst, ee_uint8_t selector) {

    const ee_x86_instruction_type_t* pseudonyms = 0;
    ee_size_t num_pseudonyms = 0;

    switch (inst) {

    case EE_X86_INSTRUCTION_CMPPS:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPPS;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPPS);
        break;

    case EE_X86_INSTRUCTION_CMPPD:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPPD;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPPD);
        break;

    case EE_X86_INSTRUCTION_CMPSD:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPSD;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPSD);
        break;

    case EE_X86_INSTRUCTION_CMPSS:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPSS;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_CMPSS);
        break;

    case EE_X86_INSTRUCTION_VCMPPS:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPPS;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPPS);
        break;

    case EE_X86_INSTRUCTION_VCMPPD:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPPD;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPPD);
        break;

    case EE_X86_INSTRUCTION_VCMPSD:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPSD;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPSD);
        break;

    case EE_X86_INSTRUCTION_VCMPSS:

        pseudonyms = EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPSS;
        num_pseudonyms = EE_GET_ARRAY_LEN(EE_PRV_X86_INSTRUCTION_PSEUDONYMS_VCMPSS);
        break;

    default:

        break;
    }

    if (!pseudonyms || selector >= num_pseudonyms)
        return EE_X86_INSTRUCTION_NOT_EXISTING;

    return pseudonyms[selector];
}

static ee_bool_t ee_prv_x86_state_pseudonymize(ee_prv_x86_state_t* distate) {

    ee_x86_instruction_type_t pseudonym = EE_X86_INSTRUCTION_NOT_EXISTING;
    ee_size_t new_num_operands = 0;

    if (distate->num_operands >= 3) {

        const ee_size_t last_opd_index = distate->num_operands - 1;
        if (distate->operands[last_opd_index].type == EE_X86_OPERAND_IMMEDIATE_VALUE) {

            pseudonym = ee_prv_x86_lookup_instruction_pseudonym(distate->instruction, (ee_uint8_t)distate->operands[last_opd_index].un.immediate_value.value);
            if (pseudonym == EE_X86_INSTRUCTION_NOT_EXISTING) {

                /* Failure during instruction pseudonym lookup is not a hard error. If this fails, we simply refrain from pseudonymization. */
                return EE_TRUE;
            }

            new_num_operands = last_opd_index;
        }
    }

    distate->instruction = pseudonym;
    distate->num_operands = new_num_operands;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86_state_process_operand_bytes_wrapped(ee_prv_x86_state_t* distate) {

    if (!distate->process_operand_bytes)
        return EE_TRUE;

    if (!distate->process_operand_bytes(distate))
        return EE_FALSE;

    return EE_TRUE;
}

ee_bool_t ee_x86_disasm(ee_x86_mode_t mode, const ee_byte_t* bytes, ee_size_t num_bytes, ee_x86_disasm_output_t* output) {

    ee_prv_x86_state_t distate = { 0 };
    const ee_prv_x86_opcode_identity_list_t* opcode_identity_list = 0;

    if (!bytes || !num_bytes || !output)
        return EE_FALSE;

    distate.active_mode = mode;
    distate.bytes = bytes;
    distate.num_bytes = num_bytes;

    /* 1) Collect prefix bytes. */
    if (!ee_prv_x86_collect_prefixes(&distate))
        return EE_FALSE;

    /* 2) Collect opcode identities. */
    if (!ee_prv_x86_collect_opcode_identity_list(&distate, &opcode_identity_list))
        return EE_FALSE;

    /* 3) Identify opcode. */
    if (!ee_prv_x86_identify_opcode(&distate, opcode_identity_list))
        return EE_FALSE;

    /* 4) Process operand bytes. */
    if (!ee_prv_x86_state_process_operand_bytes_wrapped(&distate))
        return EE_FALSE;

    /* 5) Identify printable prefixes (lock & repeat, branch hint). */
    ee_prv_x86_identify_printable_prefixes(&distate);

    /* 6) Pseudonymize the disasm state (if eligible). */
    if (distate.instruction_flags.is_eligible_for_pseudonymization && !ee_prv_x86_state_pseudonymize(&distate))
        return EE_FALSE;

    /* 7) Write output. */
    output->num_instruction_bytes = distate.byte_index;
    output->instruction = distate.instruction;
    output->num_operands = distate.num_operands;

    ee_memcpy(&output->prefixes, &distate.final_prefixes, sizeof(output->prefixes));
    ee_memcpy(output->operands, distate.operands, sizeof(output->operands));

    return EE_TRUE;
}

#endif
