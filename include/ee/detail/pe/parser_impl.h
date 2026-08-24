#ifndef EE_PE_PARSER_IMPL_IG
#define EE_PE_PARSER_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/pe/types.h"

EE_STATIC_ASSERT(sizeof(ee_prv_pe_rii_state_t) < 2048, pe_rii_state_must_be_smaller_than_2048b);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_coff_file_header_t) == EE_PRV_PE_SIZE_COFF_FILE_HEADER, pe_coff_file_header_size_mismatch);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_optional_header_t) == EE_PRV_PE_SIZE_OPTIONAL_HEADER, pe_optional_header_size_mismatch);
EE_STATIC_ASSERT(EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_pe_optional_header_t, data_dirs) == EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_pe_unified_optional_header_t, data_dirs), pe_data_dirs_size_mismatch);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_optional_header_plus_t) == EE_PRV_PE_SIZE_OPTIONAL_HEADER_PLUS, pe_optional_header_plus_size_mismatch);
EE_STATIC_ASSERT(EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_pe_optional_header_plus_t, data_dirs) == EE_GET_TDEF_STRUCT_FIELD_LEN(ee_prv_pe_unified_optional_header_t, data_dirs), pe_optional_header_plus_data_dirs_size_mismatch);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_section_header_t) == EE_PRV_PE_SIZE_SECTION_HEADER, pe_section_header_size_mismatch);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_import_directory_entry_t) == EE_PRV_PE_SIZE_IMPORT_DIRECTORY_ENTRY, pe_import_dir_entry_size_mismatch);
EE_STATIC_ASSERT(sizeof(ee_prv_pe_export_directory_entry_t) == EE_PRV_PE_SIZE_EXPORT_DIRECTORY_ENTRY, pe_export_dir_entry_size_mismatch);

static void ee_prv_pe_free_symbols(ee_pe_symbol_info_t* symbols, ee_size_t num_symbols, ee_free_t free) {

    ee_size_t index = 0;

    if (!symbols)
        return;

    for (; index != num_symbols; ++index) {

        const ee_pe_symbol_info_t* const symbol = &symbols[index];
        if (symbol->name)
            free(symbol->name);

        if (symbol->forwarder_info)
            free(symbol->forwarder_info);
    }

    free(symbols);
}

static void ee_prv_pe_free_imports(ee_pe_import_info_t* imports, ee_size_t num_imports, ee_free_t free) {

    ee_size_t index = 0;

    if (!imports)
        return;

    for (; index != num_imports; ++index) {

        const ee_pe_import_info_t* const import = &imports[index];
        if (import->module_name)
            free(import->module_name);

        if (import->symbols)
            ee_prv_pe_free_symbols(import->symbols, import->num_symbols, free);
    }

    free(imports);
}

static void ee_prv_pe_rii_state_free_fields(const ee_prv_pe_rii_state_t* state) {

    const ee_pe_rii_callbacks_t* cbs = state->callbacks;

    if (state->section_headers)
        cbs->free(state->section_headers);

    if (state->imports)
        ee_prv_pe_free_imports(state->imports, state->num_imports, cbs->free);

    if (state->exports)
        ee_prv_pe_free_symbols(state->exports, state->num_exports, cbs->free);
}

static ee_pe_error_t ee_prv_pe_rii_state_read_coff_file_header(ee_prv_pe_rii_state_t* state) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_uint32_t image_sig_fp = 0;
    ee_uint32_t image_sig = 0;

    state->cur_fp = EE_PRV_PE_FILE_OFFSET_IMAGE_SIGNATURE;
    if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(image_sig_fp), &image_sig_fp))
        return EE_PE_MALFORMED_FILE;

    state->cur_fp = image_sig_fp;
    if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(image_sig), &image_sig))
        return EE_PE_MALFORMED_FILE;

    if (image_sig != EE_PRV_PE_SIGNATURE_IMAGE_FILE)
        return EE_PE_MALFORMED_FILE;

    state->cur_fp += sizeof(image_sig);
    if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(state->coff_file_header), &state->coff_file_header))
        return EE_PE_MALFORMED_FILE;

    state->cur_fp += sizeof(state->coff_file_header);
    return EE_PE_SUCCESS;
}

static void ee_prv_pe_convert_optional_header_to_unified_optional_header(const ee_prv_pe_optional_header_t* opt_header, ee_prv_pe_unified_optional_header_t* unified_opt_header) {

    unified_opt_header->is_pe_plus_header = EE_FALSE;

    unified_opt_header->major_linker_version = opt_header->major_linker_version;
    unified_opt_header->minor_linker_version = opt_header->minor_linker_version;
    unified_opt_header->size_of_code = opt_header->size_of_code;
    unified_opt_header->size_of_initialized_data = opt_header->size_of_initialized_data;
    unified_opt_header->size_of_uninitialized_data = opt_header->size_of_uninitialized_data;
    unified_opt_header->address_of_entry_point = opt_header->address_of_entry_point;
    unified_opt_header->base_of_code = opt_header->base_of_code;
    unified_opt_header->base_of_data = opt_header->base_of_data;
    unified_opt_header->image_base = opt_header->image_base;
    unified_opt_header->section_alignment = opt_header->section_alignment;
    unified_opt_header->file_alignment = opt_header->file_alignment;
    unified_opt_header->major_os_version = opt_header->major_os_version;
    unified_opt_header->minor_os_version = opt_header->minor_os_version;
    unified_opt_header->major_image_version = opt_header->major_image_version;
    unified_opt_header->minor_image_version = opt_header->minor_image_version;
    unified_opt_header->major_subsystem_version = opt_header->major_subsystem_version;
    unified_opt_header->minor_subsystem_version = opt_header->minor_subsystem_version;
    unified_opt_header->win32_version_value = opt_header->win32_version_value;
    unified_opt_header->size_of_image = opt_header->size_of_image;
    unified_opt_header->size_of_headers = opt_header->size_of_headers;
    unified_opt_header->check_sum = opt_header->check_sum;
    unified_opt_header->subsystem = opt_header->subsystem;
    unified_opt_header->dll_characteristics = opt_header->dll_characteristics;
    unified_opt_header->size_of_stack_reserve = opt_header->size_of_stack_reserve;
    unified_opt_header->size_of_stack_commit = opt_header->size_of_stack_commit;
    unified_opt_header->size_of_heap_reserve = opt_header->size_of_heap_reserve;
    unified_opt_header->size_of_heap_commit = opt_header->size_of_heap_commit;
    unified_opt_header->loader_flags = opt_header->loader_flags;
    unified_opt_header->number_of_rva_and_sizes = opt_header->number_of_rva_and_sizes;

    ee_memcpy(unified_opt_header->data_dirs, opt_header->data_dirs, sizeof(unified_opt_header->data_dirs));
}

static void ee_prv_pe_convert_optional_header_plus_to_unified_optional_header(const ee_prv_pe_optional_header_plus_t* opt_header, ee_prv_pe_unified_optional_header_t* unified_opt_header) {

    unified_opt_header->is_pe_plus_header = EE_TRUE;

    unified_opt_header->major_linker_version = opt_header->major_linker_version;
    unified_opt_header->minor_linker_version = opt_header->minor_linker_version;
    unified_opt_header->size_of_code = opt_header->size_of_code;
    unified_opt_header->size_of_initialized_data = opt_header->size_of_initialized_data;
    unified_opt_header->size_of_uninitialized_data = opt_header->size_of_uninitialized_data;
    unified_opt_header->address_of_entry_point = opt_header->address_of_entry_point;
    unified_opt_header->base_of_code = opt_header->base_of_code;
    unified_opt_header->base_of_data = 0;
    unified_opt_header->image_base = opt_header->image_base;
    unified_opt_header->section_alignment = opt_header->section_alignment;
    unified_opt_header->file_alignment = opt_header->file_alignment;
    unified_opt_header->major_os_version = opt_header->major_os_version;
    unified_opt_header->minor_os_version = opt_header->minor_os_version;
    unified_opt_header->major_image_version = opt_header->major_image_version;
    unified_opt_header->minor_image_version = opt_header->minor_image_version;
    unified_opt_header->major_subsystem_version = opt_header->major_subsystem_version;
    unified_opt_header->minor_subsystem_version = opt_header->minor_subsystem_version;
    unified_opt_header->win32_version_value = opt_header->win32_version_value;
    unified_opt_header->size_of_image = opt_header->size_of_image;
    unified_opt_header->size_of_headers = opt_header->size_of_headers;
    unified_opt_header->check_sum = opt_header->check_sum;
    unified_opt_header->subsystem = opt_header->subsystem;
    unified_opt_header->dll_characteristics = opt_header->dll_characteristics;
    unified_opt_header->size_of_stack_reserve = opt_header->size_of_stack_reserve;
    unified_opt_header->size_of_stack_commit = opt_header->size_of_stack_commit;
    unified_opt_header->size_of_heap_reserve = opt_header->size_of_heap_reserve;
    unified_opt_header->size_of_heap_commit = opt_header->size_of_heap_commit;
    unified_opt_header->loader_flags = opt_header->loader_flags;
    unified_opt_header->number_of_rva_and_sizes = opt_header->number_of_rva_and_sizes;

    ee_memcpy(unified_opt_header->data_dirs, opt_header->data_dirs, sizeof(unified_opt_header->data_dirs));
}

static ee_pe_error_t ee_prv_pe_rii_state_read_optional_header(ee_prv_pe_rii_state_t* state) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_uint16_t opt_header_magic = 0;
    ee_size_t opt_header_size = 0;

    if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(opt_header_magic), &opt_header_magic))
        return EE_PE_MALFORMED_FILE;

    if (opt_header_magic == EE_PRV_PE_MAGIC_OPTIONAL_HEADER) {

        ee_prv_pe_optional_header_t opt_header = { 0 };

        if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(opt_header), &opt_header))
            return EE_PE_MALFORMED_FILE;

        ee_prv_pe_convert_optional_header_to_unified_optional_header(&opt_header, &state->opt_header);
        opt_header_size = sizeof(opt_header);
    }
    else if (opt_header_magic == EE_PRV_PE_MAGIC_OPTIONAL_HEADER_PLUS) {

        ee_prv_pe_optional_header_plus_t opt_header = { 0 };

        if (!read_data->cb(read_data->ctx, state->cur_fp, sizeof(opt_header), &opt_header))
            return EE_PE_MALFORMED_FILE;

        ee_prv_pe_convert_optional_header_plus_to_unified_optional_header(&opt_header, &state->opt_header);
        opt_header_size = sizeof(opt_header);
    }
    else {

        return EE_PE_MALFORMED_FILE;
    }

    state->cur_fp += (ee_uint32_t)opt_header_size;
    return EE_PE_SUCCESS;
}

static const ee_prv_pe_data_dir_t* ee_prv_pe_rii_state_get_data_dir(ee_prv_pe_rii_state_t* state, ee_prv_pe_data_dir_index_t index) {

    const ee_size_t index_val = (ee_size_t)index;

    if (index_val >= state->opt_header.number_of_rva_and_sizes)
        return 0;

    return &state->opt_header.data_dirs[index_val];
}

static ee_pe_error_t ee_prv_pe_rii_state_read_section_table(ee_prv_pe_rii_state_t* state) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_size_t section_headers_size = 0;

    state->num_section_headers = state->coff_file_header.number_of_sections;
    state->section_headers = (ee_prv_pe_section_header_t*)cbs->calloc(state->num_section_headers, sizeof(ee_prv_pe_section_header_t));

    if (!state->section_headers)
        return EE_PE_BAD_ALLOC;

    section_headers_size = state->num_section_headers * sizeof(ee_prv_pe_section_header_t);
    if (!read_data->cb(read_data->ctx, state->cur_fp, section_headers_size, state->section_headers))
        return EE_PE_MALFORMED_FILE;

    state->cur_fp += (ee_uint32_t)section_headers_size;
    return EE_PE_SUCCESS;
}

static ee_bool_t ee_prv_pe_rii_state_lookup_section_info_by_data_dir(const ee_prv_pe_rii_state_t* state, const ee_prv_pe_data_dir_t* data_dir, ee_pe_section_info_t* section_info) {

    ee_size_t hdr_index = 0;
    for (; hdr_index != state->num_section_headers; ++hdr_index) {

        const ee_prv_pe_section_header_t* const hdr = &state->section_headers[hdr_index];

        const ee_uint32_t data_dir_begin = data_dir->virtual_address;
        const ee_uint32_t data_dir_end = data_dir_begin + data_dir->size;

        const ee_uint32_t section_begin = hdr->virtual_address;
        const ee_uint32_t section_end = section_begin + hdr->virtual_size;

        if (data_dir_begin >= section_begin && data_dir_end <= section_end) {

            const ee_uint32_t offset = data_dir_begin - section_begin;

            section_info->fp = hdr->pointer_to_raw_data + offset;
            section_info->rva = hdr->virtual_address + offset;
            section_info->raw_size = section_info->loaded_size = data_dir->size;

            return EE_TRUE;
        }
    }

    return EE_FALSE;
}

static ee_bool_t ee_prv_pe_rii_state_lookup_section_info_by_data_dir_index(const ee_prv_pe_rii_state_t* state, ee_prv_pe_data_dir_index_t data_dir_index, ee_pe_section_info_t* section_info) {

    const ee_size_t index_val = (ee_size_t)data_dir_index;
    const ee_prv_pe_data_dir_t* data_dir = 0;

    if (index_val >= state->opt_header.number_of_rva_and_sizes)
        return EE_FALSE;

    data_dir = &state->opt_header.data_dirs[index_val];
    return ee_prv_pe_rii_state_lookup_section_info_by_data_dir(state, data_dir, section_info);
}

static ee_bool_t ee_prv_pe_rii_state_lookup_section_info_by_section_name(const ee_prv_pe_rii_state_t* state, const ee_ascii_char_t* name, ee_pe_section_info_t* section_info) {

    ee_size_t hdr_index = 0;
    for (; hdr_index != state->num_section_headers; ++hdr_index) {

        const ee_prv_pe_section_header_t* const hdr = &state->section_headers[hdr_index];
        if (!ee_strcmp(hdr->name, name)) {

            section_info->fp = hdr->pointer_to_raw_data;
            section_info->rva = hdr->virtual_address;
            section_info->raw_size = hdr->size_of_raw_data;
            section_info->loaded_size = hdr->virtual_size;

            return EE_TRUE;
        }
    }

    return EE_FALSE;
}

static void ee_prv_pe_rii_state_collect_section_info(ee_prv_pe_rii_state_t* state) {

    ee_bool_t idata_found = EE_FALSE;
    ee_bool_t edata_found = EE_FALSE;

    ee_pe_section_info_t section_info = { 0 };

    ee_ascii_char_t deob_buf[64] = { 0 };
    const ee_size_t deob_buf_size = sizeof(deob_buf);

    /* Lookup rdata, data and text section. */
    ee_prv_pe_rii_state_lookup_section_info_by_section_name(state, EE_OBFUSCATED_SL("\xE2\xBE\x31\x35\x9C\x65", deob_buf, deob_buf_size), &state->rdata_section_info);
    ee_prv_pe_rii_state_lookup_section_info_by_section_name(state, EE_OBFUSCATED_SL("\xE2\xA8\x34\x20\x89", deob_buf, deob_buf_size), &state->data_section_info);
    ee_prv_pe_rii_state_lookup_section_info_by_section_name(state, EE_OBFUSCATED_SL("\xE2\xB8\x30\x2C\x9C", deob_buf, deob_buf_size), &state->text_section_info);

    /* Lookup idata. */
    idata_found = ee_prv_pe_rii_state_lookup_section_info_by_data_dir_index(state, EE_PRV_PE_DATA_DIR_INDEX_IMPORT_TABLE, &section_info)
        || ee_prv_pe_rii_state_lookup_section_info_by_section_name(state, EE_OBFUSCATED_SL("\xE2\xA5\xA8\x34\x20\x89", deob_buf, deob_buf_size), &section_info);

    if (idata_found)
        state->idata_section_info = section_info;

    /* Lookup IAT. */
    if (ee_prv_pe_rii_state_lookup_section_info_by_data_dir_index(state, EE_PRV_PE_DATA_DIR_INDEX_IAT, &section_info))
        state->iat_section_info = section_info;

    /* Lookup edata. */
    edata_found = ee_prv_pe_rii_state_lookup_section_info_by_data_dir_index(state, EE_PRV_PE_DATA_DIR_INDEX_EXPORT_TABLE, &section_info)
        || ee_prv_pe_rii_state_lookup_section_info_by_section_name(state, EE_OBFUSCATED_SL("\xE2\xA9\x31\x35\x9C\x65", deob_buf, deob_buf_size), &section_info);

    if (edata_found)
        state->edata_section_info = section_info;
}

static ee_bool_t ee_prv_pe_rii_state_lookup_fp_by_rva(ee_prv_pe_rii_state_t* state, ee_uint32_t rva, ee_uint32_t* fp) {

    ee_size_t hdr_index = 0;
    for (; hdr_index != state->num_section_headers; ++hdr_index) {

        const ee_prv_pe_section_header_t* const hdr = &state->section_headers[hdr_index];

        const ee_uint32_t section_begin = hdr->virtual_address;
        const ee_uint32_t section_end = section_begin + hdr->virtual_size;

        if (rva >= section_begin && rva < section_end) {

            const ee_uint32_t offset = rva - section_begin;
            *fp = hdr->pointer_to_raw_data + offset;

            return EE_TRUE;
        }
    }

    return EE_FALSE;
}

static ee_int32_t ee_prv_pe_strlen_with_limit(const ee_ascii_char_t* str, ee_size_t str_size) {

    ee_size_t char_index = 0;

    if (!str_size)
        return 0;

    for (; char_index != str_size; ++char_index) {

        if (!str[char_index])
            return (ee_int32_t)char_index;
    }

    return -1;
}

static ee_pe_error_t ee_prv_pe_read_string(const ee_read_data_wrapper_t* read_data, ee_uint32_t fp, ee_ascii_char_t* str, ee_size_t str_size) {

    ee_uint32_t cur_fp = fp;
    ee_ascii_char_t str_chunk[64] = { 0 };
    ee_size_t char_index = 0;

    /* 1) Read string chunk-wise.
    */

    while (read_data->cb(read_data->ctx, cur_fp + (ee_uint32_t)char_index, sizeof(str_chunk), str_chunk)) {

        const ee_int32_t str_chunk_len = ee_prv_pe_strlen_with_limit(str_chunk, sizeof(str_chunk));
        const ee_size_t actual_str_chunk_len = (str_chunk_len < 0) ? sizeof(str_chunk) : (ee_size_t)str_chunk_len;

        if (char_index + actual_str_chunk_len >= str_size)
            return EE_PE_PREDEFINED_LIMIT_REACHED;

        ee_memcpy(&str[char_index], str_chunk, actual_str_chunk_len);
        char_index += actual_str_chunk_len;

        if (str_chunk_len >= 0) {

            str[char_index] = 0;
            return EE_PE_SUCCESS;
        }
    }

    /* 2) Read potentially remaining characters byte by byte.
    */

    for (; char_index < str_size; ++char_index) {

        ee_ascii_char_t cur_char = 0;
        if (!read_data->cb(read_data->ctx, cur_fp + (ee_uint32_t)char_index, sizeof(cur_char), &cur_char))
            return EE_PE_MALFORMED_FILE;

        str[char_index] = cur_char;
        if (!cur_char)
            return EE_PE_SUCCESS;
    }

    return EE_PE_PREDEFINED_LIMIT_REACHED;
}

static ee_bool_t ee_prv_pe_section_info_is_valid(const ee_pe_section_info_t* info) {
    return info && info->raw_size;
}

static ee_pe_error_t ee_prv_pe_rii_state_collect_imported_symbol_info(ee_prv_pe_rii_state_t* state, const ee_prv_pe_imported_dll_info_t* dll_info, const ee_prv_pe_import_lookup_table_entry_info_t* ilt_entry_info, ee_pe_symbol_info_t* out) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    const ee_uint64_t import_by_ordinal_bit = ilt_entry_info->size == 4 ? 0x80000000 : 0x8000000000000000;
    const ee_uint32_t ilt_entry_offset = (ee_uint32_t)ilt_entry_info->index * (ee_uint32_t)ilt_entry_info->size;
    ee_uint64_t iat_entry = 0;

    if (ilt_entry_info->value & import_by_ordinal_bit) {

        out->biased_ordinal = (ee_uint16_t)ilt_entry_info->value;
    }
    else {

        const ee_uint32_t name_table_rva = (ee_uint32_t)ilt_entry_info->value & 0x7fffffff;
        ee_uint32_t name_table_fp = 0;

        ee_pe_error_t err = EE_PE_FAILURE;

        ee_uint32_t symbol_name_fp = 0;
        ee_ascii_char_t symbol_name[EE_PRV_PE_LIMIT_MAX_STRING_LENGTH] = { 0 };
        ee_size_t symbol_name_len = 0;

        out->biased_ordinal = EE_PE_ORDINAL_UNUSED;

        if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, name_table_rva, &name_table_fp))
            return EE_PE_MALFORMED_FILE;

        /* Skip hint field. */
        symbol_name_fp = name_table_fp + 2;

        err = ee_prv_pe_read_string(read_data, symbol_name_fp, symbol_name, sizeof(symbol_name));
        if (err != EE_PE_SUCCESS)
            return err;

        symbol_name_len = ee_strlen(symbol_name);
        if (!symbol_name_len)
            return EE_PE_MALFORMED_FILE;

        out->name = (ee_ascii_char_t*)cbs->calloc(symbol_name_len + 1, sizeof(ee_ascii_char_t));
        if (!out->name)
            return EE_PE_BAD_ALLOC;

        ee_memcpy(out->name, symbol_name, symbol_name_len);
    }

    out->fp = dll_info->iat_fp + ilt_entry_offset;
    out->rva = dll_info->iat_rva + ilt_entry_offset;

    if (ilt_entry_info->size > sizeof(iat_entry))
        return EE_PE_FAILURE;

    if (!read_data->cb(read_data->ctx, out->fp, ilt_entry_info->size, &iat_entry))
        return EE_PE_MALFORMED_FILE;

    return EE_PE_SUCCESS;
}

static ee_pe_error_t ee_prv_pe_rii_state_collect_imports_from_entry(ee_prv_pe_rii_state_t* state, const ee_prv_pe_import_directory_entry_t* entry, ee_pe_import_info_t* out) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_pe_error_t err = EE_PE_FAILURE;
    ee_prv_pe_imported_dll_info_t dll_info = { 0 };

    ee_uint32_t module_name_fp = 0;
    ee_uint32_t ilt_fp = 0;
    ee_uint32_t iat_fp = 0;
    ee_uint32_t cur_fp = 0;
    ee_uint64_t cur_ilt_entry = 0;

    ee_size_t ilt_entry_size_bytes = 0;
    ee_size_t num_symbols = 0;
    ee_size_t symbol_index = 0;

    ee_ascii_char_t module_name[EE_PRV_PE_LIMIT_MAX_STRING_LENGTH] = { 0 };

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, entry->name, &module_name_fp))
        return EE_PE_MALFORMED_FILE;

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, entry->import_lookup_table, &ilt_fp))
        return EE_PE_MALFORMED_FILE;

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, entry->import_address_table, &iat_fp))
        return EE_PE_MALFORMED_FILE;

    dll_info.import_lookup_table_fp = ilt_fp;
    dll_info.import_lookup_table_rva = entry->import_lookup_table;
    dll_info.iat_fp = iat_fp;
    dll_info.iat_rva = entry->import_address_table;

    err = ee_prv_pe_read_string(read_data, module_name_fp, module_name, sizeof(module_name));
    if (err != EE_PE_SUCCESS)
        return err;

    if (!state->opt_header.is_pe_plus_header)
        ilt_entry_size_bytes = 4;
    else
        ilt_entry_size_bytes = 8;

    out->module_name = (ee_ascii_char_t*)cbs->calloc(ee_strlen(module_name) + 1, sizeof(ee_ascii_char_t));
    if (!out->module_name)
        return EE_PE_BAD_ALLOC;

    ee_memcpy(out->module_name, module_name, ee_strlen(module_name));

    /* 1) Determine number of imported symbols and allocate memory. */
    cur_fp = ilt_fp;
    for (; num_symbols != EE_PRV_PE_LIMIT_MAX_IMPORTED_SYMBOLS_PER_DLL; ++num_symbols) {

        if (!read_data->cb(read_data->ctx, cur_fp, ilt_entry_size_bytes, &cur_ilt_entry))
            return EE_PE_MALFORMED_FILE;

        if (!cur_ilt_entry)
            break;

        cur_fp += (ee_uint32_t)ilt_entry_size_bytes;
    }

    if (!num_symbols)
        return EE_PE_SUCCESS;

    if (num_symbols == EE_PRV_PE_LIMIT_MAX_IMPORTED_SYMBOLS_PER_DLL)
        return EE_PE_PREDEFINED_LIMIT_REACHED;

    out->symbols = (ee_pe_symbol_info_t*)cbs->calloc(num_symbols, sizeof(ee_pe_symbol_info_t));
    if (!out->symbols)
        return EE_PE_BAD_ALLOC;
    
    out->num_symbols = num_symbols;

    /* 2) Parse imported symbols. */
    cur_fp = ilt_fp;
    for (; symbol_index != num_symbols; ++symbol_index) {

        ee_pe_error_t err = EE_PE_FAILURE;
        ee_prv_pe_import_lookup_table_entry_info_t ilt_entry_info = { 0 };

        if (!read_data->cb(read_data->ctx, cur_fp, ilt_entry_size_bytes, &cur_ilt_entry))
            return EE_PE_MALFORMED_FILE;

        ilt_entry_info.index = symbol_index;
        ilt_entry_info.value = cur_ilt_entry;
        ilt_entry_info.size = ilt_entry_size_bytes;

        err = ee_prv_pe_rii_state_collect_imported_symbol_info(state, &dll_info, &ilt_entry_info, &out->symbols[symbol_index]);
        if (err != EE_PE_SUCCESS)
            return err;

        cur_fp += (ee_uint32_t)ilt_entry_size_bytes;
    }

    return EE_PE_SUCCESS;
}

static ee_pe_error_t ee_prv_pe_rii_state_collect_imports(ee_prv_pe_rii_state_t* state) {

    const ee_pe_section_info_t* const idata = &state->idata_section_info;
    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_uint32_t cur_fp = 0;
    ee_size_t num_dlls = 0;
    ee_size_t dll_index = 0;

    ee_prv_pe_import_directory_entry_t cur_entry = { 0 };
    ee_prv_pe_import_directory_entry_t null_entry = { 0 };

    if (!idata->raw_size)
        return EE_PE_SUCCESS;

    /* 1) Determine number of imported DLLs and allocate memory. */
    cur_fp = idata->fp;
    for (; num_dlls != EE_PRV_PE_LIMIT_MAX_IMPORTED_DLLS; ++num_dlls) {

        if (!read_data->cb(read_data->ctx, cur_fp, sizeof(cur_entry), &cur_entry))
            return EE_PE_MALFORMED_FILE;

        if (!ee_memcmp(&cur_entry, &null_entry, sizeof(cur_entry)))
            break;

        cur_fp += sizeof(cur_entry);
    }

    if (!num_dlls)
        return EE_PE_SUCCESS;

    if (num_dlls == EE_PRV_PE_LIMIT_MAX_IMPORTED_DLLS)
        return EE_PE_PREDEFINED_LIMIT_REACHED;

    state->imports = (ee_pe_import_info_t*)cbs->calloc(num_dlls, sizeof(ee_pe_import_info_t));
    if (!state->imports)
        return EE_PE_BAD_ALLOC;

    state->num_imports = num_dlls;

    /* 2) Parse imported DLL entries. */
    cur_fp = idata->fp;
    for (; dll_index != num_dlls; ++dll_index) {

        ee_pe_error_t err = EE_PE_FAILURE;

        if (!read_data->cb(read_data->ctx, cur_fp, sizeof(cur_entry), &cur_entry))
            return EE_PE_MALFORMED_FILE;

        err = ee_prv_pe_rii_state_collect_imports_from_entry(state, &cur_entry, &state->imports[dll_index]);
        if (err != EE_PE_SUCCESS)
            return err;

        cur_fp += sizeof(cur_entry);
    }

    return EE_PE_SUCCESS;
}

static ee_pe_error_t ee_prv_pe_rii_state_collect_exported_symbol_info(ee_prv_pe_rii_state_t* state, const ee_prv_pe_export_directory_info_t* export_dir_info, ee_size_t symbol_index, ee_pe_symbol_info_t* out) {

    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_uint16_t unbiased_ordinal = 0;
    ee_uint32_t name_table_entry = 0;
    ee_uint32_t addr_table_entry = 0;

    const ee_uint32_t ordinal_table_entry_fp = export_dir_info->ordinal_table_fp + (ee_uint32_t)symbol_index * sizeof(unbiased_ordinal);
    const ee_uint32_t name_table_entry_fp = export_dir_info->name_table_fp + (ee_uint32_t)symbol_index * sizeof(name_table_entry);
    ee_uint32_t addr_table_entry_fp = 0;

    const ee_uint32_t edata_begin_rva = state->edata_section_info.rva;
    const ee_uint32_t edata_end_rva = edata_begin_rva + state->edata_section_info.loaded_size;

    ee_pe_error_t err = EE_PE_FAILURE;

    ee_uint32_t name_fp = 0;
    ee_ascii_char_t name[EE_PRV_PE_LIMIT_MAX_STRING_LENGTH] = { 0 };
    ee_size_t name_len = 0;

    if (!read_data->cb(read_data->ctx, ordinal_table_entry_fp, sizeof(unbiased_ordinal), &unbiased_ordinal))
        return EE_PE_MALFORMED_FILE;

    if (unbiased_ordinal >= export_dir_info->num_addr_table_entries)
        return EE_PE_MALFORMED_FILE;

    if (!read_data->cb(read_data->ctx, name_table_entry_fp, sizeof(name_table_entry), &name_table_entry))
        return EE_PE_MALFORMED_FILE;

    addr_table_entry_fp = export_dir_info->addr_table_fp + unbiased_ordinal * sizeof(addr_table_entry);

    if (!read_data->cb(read_data->ctx, addr_table_entry_fp, sizeof(addr_table_entry), &addr_table_entry))
        return EE_PE_MALFORMED_FILE;

    if (addr_table_entry >= edata_begin_rva && addr_table_entry < edata_end_rva) {

        /* Scenario 1: Entry is a forwarder RVA. */
        ee_uint32_t forwarder_info_fp = 0;
        ee_ascii_char_t forwarder_info[EE_PRV_PE_LIMIT_MAX_STRING_LENGTH] = { 0 };
        ee_size_t forwarder_info_len = 0;

        if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, addr_table_entry, &forwarder_info_fp))
            return EE_PE_MALFORMED_FILE;

        err = ee_prv_pe_read_string(read_data, forwarder_info_fp, forwarder_info, sizeof(forwarder_info));
        if (err != EE_PE_SUCCESS)
            return err;

        forwarder_info_len = ee_strlen(forwarder_info);
        out->forwarder_info = (ee_ascii_char_t*)cbs->calloc(forwarder_info_len, sizeof(ee_ascii_char_t) + 1);

        if (!out->forwarder_info)
            return EE_PE_BAD_ALLOC;

        ee_memcpy(out->forwarder_info, forwarder_info, forwarder_info_len);
    }
    else {

        /* Scenario 2: Entry is "regular" export RVA. */
        ee_uint32_t symbol_fp = 0;
        if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, addr_table_entry, &symbol_fp))
            return EE_PE_MALFORMED_FILE;

        out->fp = symbol_fp;
        out->rva = addr_table_entry;
    }

    out->biased_ordinal = (ee_uint16_t)(export_dir_info->ordinal_base + unbiased_ordinal);

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, name_table_entry, &name_fp))
        return EE_PE_MALFORMED_FILE;

    err = ee_prv_pe_read_string(read_data, name_fp, name, sizeof(name));
    if (err != EE_PE_SUCCESS)
        return err;

    name_len = ee_strlen(name);
    out->name = (ee_ascii_char_t*)cbs->calloc(name_len, sizeof(ee_ascii_char_t) + 1);

    if (!out->name)
        return EE_PE_BAD_ALLOC;

    ee_memcpy(out->name, name, name_len);
    return EE_PE_SUCCESS;
}

static ee_pe_error_t ee_prv_pe_rii_state_collect_exports(ee_prv_pe_rii_state_t* state) {

    const ee_pe_section_info_t* const edata = &state->edata_section_info;
    const ee_pe_rii_callbacks_t* const cbs = state->callbacks;
    const ee_read_data_wrapper_t* const read_data = &cbs->read_data;

    ee_prv_pe_export_directory_entry_t dir_entry = { 0 };
    ee_prv_pe_export_directory_info_t dir_info = { 0 };
    ee_size_t export_index = 0;

    ee_pe_error_t err = EE_PE_FAILURE;

    if (!edata->raw_size)
        return EE_PE_SUCCESS;

    if (!read_data->cb(read_data->ctx, edata->fp, sizeof(dir_entry), &dir_entry))
        return EE_PE_MALFORMED_FILE;

    if (!dir_entry.number_of_functions)
        return EE_PE_SUCCESS;

    if (dir_entry.number_of_functions >= EE_PRV_PE_LIMIT_MAX_EXPORTED_SYMBOLS)
        return EE_PE_PREDEFINED_LIMIT_REACHED;

    state->exports = (ee_pe_symbol_info_t*)cbs->calloc(dir_entry.number_of_names, sizeof(ee_pe_symbol_info_t));
    if (!state->exports)
        return EE_PE_BAD_ALLOC;

    state->num_exports = dir_entry.number_of_names;

    dir_info.ordinal_base = dir_entry.base;
    dir_info.num_addr_table_entries = dir_entry.number_of_functions;

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, dir_entry.address_of_functions, &dir_info.addr_table_fp))
        return EE_PE_MALFORMED_FILE;

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, dir_entry.address_of_names, &dir_info.name_table_fp))
        return EE_PE_MALFORMED_FILE;

    if (!ee_prv_pe_rii_state_lookup_fp_by_rva(state, dir_entry.address_of_name_ordinals, &dir_info.ordinal_table_fp))
        return EE_PE_MALFORMED_FILE;

    for (; export_index != state->num_exports; ++export_index) {

        err = ee_prv_pe_rii_state_collect_exported_symbol_info(state, &dir_info, export_index, &state->exports[export_index]);
        if (err != EE_PE_SUCCESS)
            return err;
    }

    return EE_PE_SUCCESS;
}

static ee_pe_machine_type_t ee_prv_pe_raw_machine_value_to_machine_type(ee_uint16_t val) {

    switch (val) {
    case EE_PRV_PE_MACHINE_I386: return EE_PE_MACHINE_I386;
    case EE_PRV_PE_MACHINE_AMD64: return EE_PE_MACHINE_AMD64;
    }

    return EE_PE_MACHINE_UNKNOWN;
}

ee_pe_error_t ee_pe_read_image_info(const ee_pe_rii_callbacks_t* callbacks, ee_pe_image_info_t** info) {

    ee_prv_pe_rii_state_t state = { 0 };
    ee_pe_error_t error = EE_PE_FAILURE;
    ee_pe_image_info_t* tmp_out = 0;

    if (!callbacks || !callbacks->calloc || !callbacks->free || !callbacks->read_data.cb || !info)
        return EE_PE_FAILURE;

    state.callbacks = callbacks;

    /* 1) Read COFF file header. */
    error = ee_prv_pe_rii_state_read_coff_file_header(&state);
    if (error != EE_PE_SUCCESS)
        goto cleanup;

    /* 2) Read optional header (which is actually not optional for image files). */
    error = ee_prv_pe_rii_state_read_optional_header(&state);
    if (error != EE_PE_SUCCESS)
        goto cleanup;

    /* 3) Read section table. */
    error = ee_prv_pe_rii_state_read_section_table(&state);
    if (error != EE_PE_SUCCESS)
        goto cleanup;

    /* 4) Collect section info. */
    ee_prv_pe_rii_state_collect_section_info(&state);

    /* 5) Collect imported symbols. */
    error = ee_prv_pe_rii_state_collect_imports(&state);
    if (error != EE_PE_SUCCESS)
        goto cleanup;

    /* 6) Collect exported symbols. */
    error = ee_prv_pe_rii_state_collect_exports(&state);
    if (error != EE_PE_SUCCESS)
        goto cleanup;

    /* 7) Write output. */
    tmp_out = (ee_pe_image_info_t*)callbacks->calloc(1, sizeof(ee_pe_image_info_t));
    if (!tmp_out)
        return EE_PE_BAD_ALLOC;

    tmp_out->machine = ee_prv_pe_raw_machine_value_to_machine_type(state.coff_file_header.machine);
    tmp_out->image_base = state.opt_header.image_base;
    tmp_out->imports = state.imports;
    tmp_out->num_imports = state.num_imports;
    tmp_out->exports = state.exports;
    tmp_out->num_exports = state.num_exports;
    tmp_out->rdata_section_info = state.rdata_section_info;
    tmp_out->data_section_info = state.data_section_info;
    tmp_out->text_section_info = state.text_section_info;

    state.imports = 0;
    state.exports = 0;

    *info = tmp_out;

cleanup:

    ee_prv_pe_rii_state_free_fields(&state);
    return error;
}

void ee_pe_free_image_info(ee_pe_image_info_t* info, ee_free_t free) {

    if (!info)
        return;

    if (info->imports)
        ee_prv_pe_free_imports(info->imports, info->num_imports, free);

    if (info->exports)
        ee_prv_pe_free_symbols(info->exports, info->num_exports, free);
}

#endif
