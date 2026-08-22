/*
This sample console application uses the ee_pe parser to get information about a user-provided PE image file (e.g. sys, dll or exe).
It is compiled with C89 language rules and does not link the C runtime (CRT-less).

This application requires 2 start arguments:

>pe_info [INFO] [FILE_PATH]

    INFO specifies the info which should be extracted (imports or exports).
    FILE_PATH points to the target file which should be parsed.

Exemplatory usages:

>pe_info imports "c:\windows\system32\notepad.exe"

    Stdout:
        (A list of all symbols imported by notepad.exe)

    Exit code: 0 (Success)

>pe_info exports "c:\windows\system32\d3d12.dll"

    Stdout:
        (A list of all symbols exported by d3d12.dll)

    Exit code: 0 (Success)
*/

#define EE_BASE_IMPL
#define EE_PE_IMPL

#include <ee/ee_pe.h>

#include "no_crt/no_crt.h"

typedef struct {

    ee_size_t size;
    ee_byte_t* content;

} file_info_t;

static ee_bool_t read_data_with_file_info_ctx(void* ctx, ee_uint32_t offset, ee_size_t size, void* out);
static void print_imports(const ee_pe_image_info_t* image_info);
static void print_exports(const ee_pe_image_info_t* image_info);

int wrapped_main(int argc, char** argv) {
    
    int exit_code = 1;
    const char* info_str = 0;
    const char* file_path = 0;

    file_info_t file_info = { 0 };
    ee_pe_rii_callbacks_t rii_cbs = { 0 };
    ee_pe_error_t rii_err = EE_PE_FAILURE;
    ee_pe_image_info_t* image_info = 0;

    if (argc < 3)
        goto cleanup;

    info_str = argv[1];
    file_path = argv[2];

    if (!nc_read_file(file_path, &file_info.content, &file_info.size))
        goto cleanup;

    /* Prepare callback data. */
    rii_cbs.calloc = nc_calloc;
    rii_cbs.free = nc_free;
    rii_cbs.read_data.cb = read_data_with_file_info_ctx;
    rii_cbs.read_data.ctx = &file_info;

    /* Call ee_pe parser. */
    rii_err = ee_pe_read_image_info(&rii_cbs, &image_info);
    if (rii_err != EE_PE_SUCCESS) {

        exit_code = (int)rii_err;
        goto cleanup;
    }

    /* Print collected data (either imports or exports). */
    if (!ee_strcmp(info_str, "imports")) {
        
        print_imports(image_info);
        exit_code = 0;
    }
    else if (!ee_strcmp(info_str, "exports")) {

        print_exports(image_info);
        exit_code = 0;
    }

cleanup:

    if (image_info)
        ee_pe_free_image_info(image_info, nc_free);

    if (file_info.content)
        nc_free(file_info.content);

    return exit_code;
}

ee_bool_t read_data_with_file_info_ctx(void* ctx, ee_uint32_t offset, ee_size_t size, void* out) {

    const file_info_t* const file_info = (file_info_t*)ctx;
    if (offset > file_info->size || size > (file_info->size - offset))
        return EE_FALSE;

    ee_memcpy(out, &file_info->content[offset], size);
    return EE_TRUE;
}

static void print_symbols(ee_bool_t requires_indent, const ee_pe_symbol_info_t* symbol_info_list, ee_size_t symbol_count) {

    ee_size_t symbol_index = 0;
    for (; symbol_index != symbol_count; ++symbol_index) {

        const ee_pe_symbol_info_t* const symbol_info = &symbol_info_list[symbol_index];
        if (requires_indent)
            nc_print("\t");

        if (symbol_info->name) {
            nc_println(symbol_info->name);
        }
        else if (symbol_info->biased_ordinal != EE_PE_ORDINAL_UNUSED) {

            ee_char8_t ordinal_str[64] = { 0 };
            if (!ee_itoa32(symbol_info->biased_ordinal, ordinal_str, sizeof(ordinal_str), EE_FALSE))
                continue;

            nc_print("#");
            nc_println(ordinal_str);
        }
    }
}

void print_imports(const ee_pe_image_info_t* image_info) {

    ee_size_t import_index = 0;
    for (; import_index != image_info->num_imports; ++import_index) {

        const ee_pe_import_info_t* const import_info = &image_info->imports[import_index];
        if (!import_info->module_name)
            nc_println("(Unknown Module)");
        else
            nc_println(import_info->module_name);

        print_symbols(EE_TRUE, import_info->symbols, import_info->num_symbols);
        
        if (import_index != (image_info->num_imports - 1))
            nc_println("");
    }
}

void print_exports(const ee_pe_image_info_t* image_info) {
    print_symbols(EE_FALSE, image_info->exports, image_info->num_exports);
}
