#ifndef EE_X86F_FORMAT_IMPL_IG
#define EE_X86F_FORMAT_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86f/lookup_impl.h"

typedef struct {

    ee_bool_t ignored;
    ee_x86_mode_t active_mode;
    ee_bool_t has_active_operand_size_override;
    ee_bool_t has_active_address_size_override;
    ee_uint64_t inst_addr;
    ee_uint64_t inst_size;

} ee_prv_x86f_base_params_t;

static ee_bool_t ee_prv_x86f_format_printable_prefixes(const ee_x86_prefix_type_t* printable_prefixes, ee_size_t num_printable_prefixes, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_size_t index = 0;

    for (; index != num_printable_prefixes; ++index) {

        const ee_x86_prefix_type_t cur_prefix = printable_prefixes[index];

        ee_ascii_char_t cur_prefix_str_buf[64] = { 0 };
        const ee_ascii_char_t* cur_prefix_str = 0;

        if (cur_prefix == EE_X86_PREFIX_NOT_EXISTING)
            break;

        cur_prefix_str = ee_prv_x86f_lookup_prefix_str(cur_prefix, cur_prefix_str_buf, sizeof(cur_prefix_str_buf));
        if (!cur_prefix_str)
            return EE_FALSE;

        if (!ee_strapp(output, output_buf_size, cur_prefix_str) || !ee_strapp(output, output_buf_size, EE_SL(" ")))
            return EE_FALSE;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86f_write_register_string(const ee_ascii_char_t* reg_prefix, ee_bool_t enclose_reg_index, ee_int32_t reg_index, const ee_ascii_char_t* reg_suffix, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    if (!ee_strapp(output, output_buf_size, reg_prefix))
        return EE_FALSE;

    if (reg_index >= 0 && reg_index < EE_X86_DEFAULT_NUMBERED_REGISTER_INDEX) {

        ee_ascii_char_t reg_index_str[4] = { 0 };
        if (!ee_itoa32(reg_index, reg_index_str, sizeof(reg_index_str), EE_FALSE))
            return EE_FALSE;

        if (enclose_reg_index && !ee_strapp(output, output_buf_size, EE_SL("(")))
            return EE_FALSE;

        if (!ee_strapp(output, output_buf_size, reg_index_str))
            return EE_FALSE;

        if (enclose_reg_index && !ee_strapp(output, output_buf_size, EE_SL(")")))
            return EE_FALSE;
    }

    if (!ee_strapp(output, output_buf_size, reg_suffix))
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86f_format_gpp_register(ee_x86_gpp_register_type_t gpp_reg, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t reg_str_buf[64] = { 0 };
    const ee_ascii_char_t* const reg_str = ee_prv_x86f_lookup_gpp_register_str(gpp_reg, reg_str_buf, sizeof(reg_str_buf));

    if (!reg_str)
        return EE_FALSE;

    return ee_prv_x86f_write_register_string(reg_str, EE_FALSE, -1, EE_SL(""), output, output_buf_size);
}

static ee_bool_t ee_prv_x86f_format_gpp_ex_register(const ee_x86_numbered_register_t* gpp_ex_reg, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    const ee_ascii_char_t* width_indicator_str = EE_SL("");
    switch (gpp_ex_reg->size_bits) {
    case 8:

        width_indicator_str = EE_SL("b");
        break;

    case 16:

        width_indicator_str = EE_SL("w");
        break;

    case 32:

        width_indicator_str = EE_SL("d");
        break;
    }

    return ee_prv_x86f_write_register_string(EE_SL("r"), EE_FALSE, gpp_ex_reg->index, width_indicator_str, output, output_buf_size);
}

static ee_bool_t ee_prv_x86f_format_vector_register(const ee_x86_numbered_register_t* vector_reg, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t deob_buf[64] = { 0 };
    const ee_size_t deob_buf_size = sizeof(deob_buf);

    switch (vector_reg->size_bits) {

    case 128:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xB4\xA1\x38", deob_buf, deob_buf_size), EE_FALSE, vector_reg->index, EE_SL(""), output, output_buf_size); /* xmm */

    case 256:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xB5\xA1\x38", deob_buf, deob_buf_size), EE_FALSE, vector_reg->index, EE_SL(""), output, output_buf_size); /* ymm */
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86f_format_segment_register(ee_x86_segment_register_type_t seg_reg, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t reg_str_buf[64] = { 0 };
    const ee_ascii_char_t* const reg_str = ee_prv_x86f_lookup_segment_register_str(seg_reg, reg_str_buf, sizeof(reg_str_buf));

    if (!reg_str)
        return EE_FALSE;

    return ee_prv_x86f_write_register_string(reg_str, EE_FALSE, -1, EE_SL(""), output, output_buf_size);
}

static ee_bool_t ee_prv_x86f_format_relative_address_operand(const ee_prv_x86f_base_params_t* base_params, const ee_x86_relative_address_operand_t* operand, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t addr_str[18] = { 0 };
    ee_ascii_char_t padded_addr_str[18] = { 0 };
    ee_size_t desired_str_len = 0;
    ee_uint64_t final_addr = base_params->inst_addr + base_params->inst_size + (ee_uint64_t)((ee_int64_t)operand->offset);

    /* Special case: For rel16 the final address must be truncated to 16 bits (outside long mode only). */
    if (base_params->active_mode != EE_X86_MODE_64 && base_params->has_active_operand_size_override)
        final_addr &= (ee_uint64_t)0xFFFF;

    if (!ee_itoa64(final_addr, addr_str, sizeof(addr_str), EE_TRUE))
        return EE_FALSE;

    switch (base_params->active_mode) {

    case EE_X86_MODE_16:

        desired_str_len = 4;
        break;

    case EE_X86_MODE_64:

        desired_str_len = 16;
        break;

    default:

        desired_str_len = 8;
    }

    /* Reminder: ee_strpad trims the string if it is longer than desired. */
    if (!ee_strpad(addr_str, desired_str_len, EE_TRUE, '0', padded_addr_str, sizeof(padded_addr_str)))
        return EE_FALSE;

    return ee_strapp(output, output_buf_size, padded_addr_str) && ee_strapp(output, output_buf_size, EE_SL("h"));
}

static ee_bool_t ee_prv_x86f_format_far_pointer_operand(const ee_x86_far_pointer_operand_t* operand, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t raw_selector_str[16] = { 0 };
    ee_ascii_char_t raw_offset_str[16] = { 0 };
    ee_ascii_char_t selector_str[16] = { 0 };
    ee_ascii_char_t offset_str[16] = { 0 };

    if (!ee_itoa64(operand->cs_selector, raw_selector_str, sizeof(raw_selector_str), EE_TRUE))
        return EE_FALSE;

    if (!ee_itoa64(operand->cs_offset, raw_offset_str, sizeof(raw_offset_str), EE_TRUE))
        return EE_FALSE;

    if (!ee_strpad(raw_selector_str, 4, EE_TRUE, '0', selector_str, sizeof(selector_str)))
        return EE_FALSE;

    if (!ee_strpad(raw_offset_str, 8, EE_TRUE, '0', offset_str, sizeof(offset_str)))
        return EE_FALSE;

    return ee_strapp(output, output_buf_size, selector_str)
        && ee_strapp(output, output_buf_size, EE_SL("h"))
        && ee_strapp(output, output_buf_size, EE_SL(":"))
        && ee_strapp(output, output_buf_size, offset_str)
        && ee_strapp(output, output_buf_size, EE_SL("h"));
}

static ee_bool_t ee_prv_x86f_format_register_operand(const ee_x86_register_operand_t* operand, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t deob_buf[64] = { 0 };
    const ee_size_t deob_buf_size = sizeof(deob_buf);

    switch (operand->type) {

    case EE_X86_REGISTER_GENERAL_PURPOSE:

        return ee_prv_x86f_format_gpp_register(operand->un.general_purpose_register, output, output_buf_size);

    case EE_X86_REGISTER_GENERAL_PURPOSE_EX:

        return ee_prv_x86f_format_gpp_ex_register(&operand->un.numbered_register, output, output_buf_size);

    case EE_X86_REGISTER_SEGMENT:

        return ee_prv_x86f_format_segment_register(operand->un.segment_register, output, output_buf_size);

    case EE_X86_REGISTER_X87:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xBF\xB8", deob_buf, deob_buf_size), EE_TRUE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* st */

    case EE_X86_REGISTER_MMX:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xA1\xA1", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* mm */

    case EE_X86_REGISTER_AVX_128:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xB4\xA1\x38", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* xmm */

    case EE_X86_REGISTER_AVX_256:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xB5\xA1\x38", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* ymm */

    case EE_X86_REGISTER_CONTROL:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xAF\xBE", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* cr */

    case EE_X86_REGISTER_DEBUG:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xA8\xBE", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* dr */
    
    case EE_X86_REGISTER_MPX:

        return ee_prv_x86f_write_register_string(EE_OBFUSCATED_SL("\xAE\xA2\x31", deob_buf, deob_buf_size), EE_FALSE, operand->un.numbered_register.index, EE_SL(""), output, output_buf_size); /* bnd */

    default:

        break;
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_x86f_format_pointer_operand_base(const ee_x86_pointer_operand_base_register_t* base, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    if (!base->is_set)
        return EE_FALSE;

    if (base->is_extended) {

        if (!ee_prv_x86f_format_gpp_ex_register(&base->un.ex_register, output, output_buf_size))
            return EE_FALSE;
    }
    else {

        if (!ee_prv_x86f_format_gpp_register(base->un.reg1ster, output, output_buf_size))
            return EE_FALSE;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86f_format_pointer_operand_index(const ee_x86_pointer_operand_index_register_t* index, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    if (!index->is_set)
        return EE_FALSE;

    if (index->is_gpp) {

        if (!ee_prv_x86f_format_gpp_register(index->un.general_purpose_register, output, output_buf_size))
            return EE_FALSE;
    }
    else if (index->is_gpp_ex) {

        if (!ee_prv_x86f_format_gpp_ex_register(&index->un.numbered_register, output, output_buf_size))
            return EE_FALSE;
    }
    else if (index->is_vec) {

        if (!ee_prv_x86f_format_vector_register(&index->un.numbered_register, output, output_buf_size))
            return EE_FALSE;
    }

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86f_format_pointer_operand_displacement(ee_bool_t is_static_address, ee_size_t disp_num_bits, ee_int64_t disp, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_int64_t actual_disp = 0;
    ee_size_t desired_disp_str_len = 0;
    ee_size_t max_disp_str_len = 0;

    ee_bool_t prepend_plus = EE_FALSE;
    ee_bool_t prepend_minus = EE_FALSE;

    ee_ascii_char_t disp_str[24] = { 0 };
    ee_size_t actual_disp_str_len = 0;

    ee_ascii_char_t resized_disp_str[24] = { 0 };

    if (is_static_address) {

        actual_disp = disp;
        desired_disp_str_len = disp_num_bits / 8 * 2;
    }
    else {

        if (disp < 0) {

            actual_disp = disp * -1;
            prepend_minus = EE_TRUE;
        }
        else {

            actual_disp = disp;
            prepend_plus = EE_TRUE;
        }

        max_disp_str_len = disp_num_bits / 8 * 2;
    }

    if (!ee_itoa64(actual_disp, disp_str, sizeof(disp_str), EE_TRUE))
        return EE_FALSE;

    actual_disp_str_len = ee_strlen(disp_str);

    if (desired_disp_str_len && actual_disp_str_len != desired_disp_str_len) {

        if (!ee_strpad(disp_str, desired_disp_str_len, EE_TRUE, '0', resized_disp_str, sizeof(resized_disp_str)))
            return EE_FALSE;
    }
    else if (max_disp_str_len && actual_disp_str_len > max_disp_str_len) {

        if (!ee_strpad(disp_str, max_disp_str_len, EE_TRUE, '0', resized_disp_str, sizeof(resized_disp_str)))
            return EE_FALSE;
    }
    else {

        ee_strncpy(resized_disp_str, disp_str, sizeof(resized_disp_str));
    }

    if (prepend_plus) {

        if (!ee_strapp(output, output_buf_size, EE_SL("+")))
            return EE_FALSE;
    }
    else if (prepend_minus) {

        if (!ee_strapp(output, output_buf_size, EE_SL("-")))
            return EE_FALSE;
    }

    return ee_strapp(output, output_buf_size, resized_disp_str) && ee_strapp(output, output_buf_size, EE_SL("h"));
}

static ee_bool_t ee_prv_x86f_format_pointer_operand(const ee_prv_x86f_base_params_t* base_params, const ee_x86_pointer_operand_t* operand, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t ptr_type_str_buf[64] = { 0 };
    const ee_ascii_char_t* const ptr_type_str = ee_prv_x86f_lookup_pointer_type_str(operand->type, ptr_type_str_buf, sizeof(ptr_type_str_buf));

    ee_ascii_char_t seg_reg_str_buf[64] = { 0 };
    const ee_ascii_char_t* seg_reg_str = ee_prv_x86f_lookup_segment_register_str(operand->segment_register, seg_reg_str_buf, sizeof(seg_reg_str_buf));

    const ee_bool_t has_base = operand->base_register.is_set || operand->index_register.is_set;

    ee_int64_t actual_disp = 0;
    ee_size_t actual_disp_num_bits = 0;

    if (!ptr_type_str)
        return EE_FALSE;

    if (!ee_strapp(output, output_buf_size, ptr_type_str))
        return EE_FALSE;

    if (seg_reg_str && (!ee_strapp(output, output_buf_size, EE_SL(" ")) || !ee_strapp(output, output_buf_size, seg_reg_str)
        || !ee_strapp(output, output_buf_size, EE_SL(":")))) {

        return EE_FALSE;
    }

    if (!ee_strapp(output, output_buf_size, EE_SL("[")))
        return EE_FALSE;

    if (operand->uses_rip_relative_addressing) {

        actual_disp = base_params->inst_addr + base_params->inst_size + (ee_uint64_t)operand->displacement;
        actual_disp_num_bits = 64;
    }
    else {

        actual_disp = operand->displacement;
        actual_disp_num_bits = operand->displacement_num_bits;

        if (operand->base_register.is_set) {

            if (!ee_prv_x86f_format_pointer_operand_base(&operand->base_register, output, output_buf_size))
                return EE_FALSE;
        }

        if (operand->index_register.is_set) {

            if (operand->base_register.is_set) {

                if (!ee_strapp(output, output_buf_size, EE_SL("+")))
                    return EE_FALSE;
            }

            if (!ee_prv_x86f_format_pointer_operand_index(&operand->index_register, output, output_buf_size))
                return EE_FALSE;

            if (operand->scaling > 1) {

                ee_ascii_char_t scaling_str[4] = { 0 };
                if (!ee_itoa32(operand->scaling, scaling_str, sizeof(scaling_str), EE_FALSE))
                    return EE_FALSE;

                if (!ee_strapp(output, output_buf_size, EE_SL("*")) || !ee_strapp(output, output_buf_size, scaling_str))
                    return EE_FALSE;
            }
        }
    }

    if (operand->displacement_num_bits && (operand->displacement || !has_base)) {

        /* Operator (+ or -) can only reliably be determined by the formatting function, hence we cannot append it beforehand. */
        if (!ee_prv_x86f_format_pointer_operand_displacement(
            !has_base,
            actual_disp_num_bits,
            actual_disp,
            output,
            output_buf_size
        )) {

            return EE_FALSE;
        }
    }

    if (!ee_strapp(output, output_buf_size, EE_SL("]")))
        return EE_FALSE;

    return EE_TRUE;
}

static ee_bool_t ee_prv_x86f_format_immediate_value_operand(const ee_x86_immediate_value_operand_t* operand, ee_ascii_char_t* output, ee_size_t output_buf_size) {

    ee_ascii_char_t imm_val_str[17] = { 0 };
    ee_ascii_char_t padded_imm_val_str[17] = { 0 };

    const ee_size_t max_str_len = operand->value_num_bits / 8 * 2;
    ee_size_t str_len = 0;
    ee_size_t desired_str_len = 0;

    if (!ee_itoa64(operand->value, imm_val_str, sizeof(imm_val_str), EE_TRUE))
        return EE_FALSE;

    str_len = ee_strlen(imm_val_str);

    if (str_len > max_str_len) {

        desired_str_len = max_str_len;
    }
    else {

        desired_str_len = str_len + str_len % 2;
    }

    if (!ee_strpad(imm_val_str, desired_str_len, EE_TRUE, '0', padded_imm_val_str, sizeof(padded_imm_val_str)))
        return EE_FALSE;

    return ee_strapp(output, output_buf_size, padded_imm_val_str) && ee_strapp(output, output_buf_size, EE_SL("h"));
}

ee_bool_t ee_x86_format(ee_x86_mode_t mode, ee_uint64_t instruction_address, const ee_x86_disasm_output_t* disasm_output, ee_ascii_char_t* format, ee_size_t* format_size) {

    ee_prv_x86f_base_params_t base_params = { 0 };

    ee_ascii_char_t tmp_format[256] = { 0 };
    ee_size_t required_format_size = 0;
    const ee_size_t available_format_size = *format_size;

    ee_ascii_char_t inst_str_buf[64] = { 0 };
    const ee_ascii_char_t* inst_str = 0;

    base_params.active_mode = mode;
    base_params.has_active_operand_size_override = disasm_output->prefixes.operand_size_override;
    base_params.has_active_address_size_override = disasm_output->prefixes.address_size_override;
    base_params.inst_addr = instruction_address;
    base_params.inst_size = disasm_output->num_instruction_bytes;

    /* 1) Process prefixes (if any). */
    if (!ee_prv_x86f_format_printable_prefixes(disasm_output->prefixes.printable_prefixes, disasm_output->prefixes.num_printable_prefixes, tmp_format, sizeof(tmp_format)))
        return EE_FALSE;

    /* 2) Process instruction. */
    inst_str = ee_prv_x86f_lookup_instruction_str(disasm_output->instruction, inst_str_buf, sizeof(inst_str_buf));
    if (!inst_str || !ee_strapp(tmp_format, sizeof(tmp_format), inst_str))
        return EE_FALSE;

    /* 3) Process operands (if any). */
    if (disasm_output->num_operands) {

        ee_size_t operand_index = 0;

        if (!ee_strapp(tmp_format, sizeof(tmp_format), EE_SL(" ")))
            return EE_FALSE;

        for (; operand_index != disasm_output->num_operands; ++operand_index) {

            const ee_x86_operand_t* const operand = &disasm_output->operands[operand_index];
            ee_bool_t operand_processed = EE_FALSE;

            switch (operand->type) {

            case EE_X86_OPERAND_RELATIVE_ADDRESS:

                operand_processed = ee_prv_x86f_format_relative_address_operand(&base_params, &operand->un.relative_address, tmp_format, sizeof(tmp_format));
                break;

            case EE_X86_OPERAND_FAR_POINTER:

                operand_processed = ee_prv_x86f_format_far_pointer_operand(&operand->un.far_pointer, tmp_format, sizeof(tmp_format));
                break;

            case EE_X86_OPERAND_REGISTER:

                operand_processed = ee_prv_x86f_format_register_operand(&operand->un.reg1ster, tmp_format, sizeof(tmp_format));
                break;

            case EE_X86_OPERAND_IMMEDIATE_VALUE:

                operand_processed = ee_prv_x86f_format_immediate_value_operand(&operand->un.immediate_value, tmp_format, sizeof(tmp_format));
                break;

            case EE_X86_OPERAND_POINTER:

                operand_processed = ee_prv_x86f_format_pointer_operand(&base_params, &operand->un.pointer, tmp_format, sizeof(tmp_format));
                break;

            default:

                break;
            }

            if (!operand_processed)
                return EE_FALSE;

            if (operand_index != disasm_output->num_operands - 1) {

                ee_ascii_char_t deob_buf[64] = { 0 };
                const ee_size_t deob_buf_size = sizeof(deob_buf);

                if (!ee_strapp(tmp_format, sizeof(tmp_format), EE_OBFUSCATED_SL("\xE0\xEC", deob_buf, deob_buf_size))) /* ", " */
                    return EE_FALSE;
            }
        }
    }

    /* 4) Verify output buffer size and write result. */
    *format_size = required_format_size = ee_strlen(tmp_format) + 1;
    if (required_format_size > available_format_size)
        return EE_FALSE;

    ee_strncpy(format, tmp_format, available_format_size);
    return EE_TRUE;
}

#endif
