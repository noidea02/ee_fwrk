/*
This sample console application uses the ee_x86 decoder & formatter to disassemble user-provided x86 instruction bytes. It is
compiled with C89 language rules and does not link the C runtime (CRT-less).

This application requires 2 start arguments:

>x86_disasm [MODE] [INSTRUCTION_BYTES]

    MODE specifies the default address size (16, 32 or 64).
    INSTRUCTION_BYTES must be a properly formatted hex sequence (e.g. \x00\x00).

Exemplatory usages:

>x86_disasm 32 "\x67\x01\x00"

    Stdout:
        add dword ptr[bx+si], eax

    Exit code: 0 (Success)

>x86_disasm 64 "\x00\x00\x48"

    Stdout:
        add byte ptr[rax], al

    Exit code: 1 (Failure due to trailing \x48 in 64-bit mode)

>x86_disasm 16 "\x02\xc0\x48"

    Stdout:
        add al, al
        dec ax

    Exit code: 0 (Success)
*/

/* These macros cause the ee library's symbols to be defined inside this compilation unit. */
#define EE_BASE_IMPL
#define EE_X86_IMPL
#define EE_X86_FORMAT_IMPL

/* Including "ee_x86_format.h" will also include all dependencies needed for x86 decoding operations. */
#include <ee/ee_x86_format.h>

/* "no_crt.h" is the CRT substitute used by this application. */
#include "no_crt/no_crt.h"

#define DISASM_BASE_ADDRESS16 0x7F00
#define DISASM_BASE_ADDRESS32 0x7FFF0000
#define DISASM_BASE_ADDRESS64 0x7FFFFFFFFFFF0000

static ee_bool_t lookup_mode(const char* str, ee_x86_mode_t* type);
static ee_bool_t hex_char_to_nibble(char chr, ee_byte_t* nibble);
static ee_bool_t hex_str_to_byte_array(const char* str, ee_byte_t* arr, ee_size_t* arr_size);
static int disassemble_and_print(ee_x86_mode_t mode, ee_byte_t* instruction, ee_size_t instruction_size);

int wrapped_main(int argc, char** argv) {
    
    int exit_code = 1;
    const char* mode_str = 0;
    const char* instruction_hex_str = 0;
    
    ee_x86_mode_t mode = EE_X86_MODE_32;
    ee_size_t inst_buf_size = 0;
    ee_byte_t* inst_buf = 0;

    if (argc < 3)
        goto cleanup;

    mode_str = argv[1];
    instruction_hex_str = argv[2];

    if (!lookup_mode(mode_str, &mode))
        goto cleanup;

    hex_str_to_byte_array(instruction_hex_str, 0, &inst_buf_size);

    inst_buf = nc_calloc(inst_buf_size, 1);
    if (!hex_str_to_byte_array(instruction_hex_str, inst_buf, &inst_buf_size))
        goto cleanup;

    /* Hand over instruction buf to ee_x86 and print results. */
    exit_code = disassemble_and_print(mode, inst_buf, inst_buf_size);

cleanup:

    if (inst_buf)
        nc_free(inst_buf);

    return exit_code;
}

ee_bool_t lookup_mode(const char* str, ee_x86_mode_t* type) {

    if (!ee_strcmp(str, "16")) {
        *type = EE_X86_MODE_16;
    }
    else if (!ee_strcmp(str, "32")) {
        *type = EE_X86_MODE_32;
    }
    else if (!ee_strcmp(str, "64")) {
        *type = EE_X86_MODE_64;
    }
    else {
        return EE_FALSE;
    }

    return EE_TRUE;
}

ee_bool_t hex_char_to_nibble(char chr, ee_byte_t* nibble) {

    if (chr >= '0' && chr <= '9') {
        
        *nibble = chr - '0';
        return EE_TRUE;
    }

    if (chr >= 'a' && chr <= 'f') {
        
        *nibble = chr - 'a' + 10;
        return EE_TRUE;
    }

    if (chr >= 'A' && chr <= 'F') {
        
        *nibble = chr - 'A' + 10;
        return EE_TRUE;
    }

    return EE_FALSE;
}

ee_bool_t hex_str_to_byte_array(const char* str, ee_byte_t* buf, ee_size_t* buf_size) {

    const ee_size_t str_len = ee_strlen(str);
    ee_size_t str_index = 0;
    ee_size_t buf_index = 0;
    ee_int32_t cur_byte = -1;
    ee_bool_t no_buf = EE_FALSE;

    for (; str_index != str_len; ++str_index) {

        const char chr = str[str_index];
        ee_byte_t nibble = 0;

        if (chr == '\\' || chr == 'x') {

            cur_byte = -1;
            continue;
        }

        if (!hex_char_to_nibble(chr, &nibble))
            return EE_FALSE;

        if (cur_byte == -1) {
            
            cur_byte = nibble;
            continue;
        }
            
        cur_byte = (cur_byte << 4) | nibble;
        
        if (buf && buf_index < *buf_size) {
            buf[buf_index] = cur_byte;
        }
        else if(!no_buf) {
            no_buf = EE_TRUE;
        }

        ++buf_index;
        cur_byte = -1;
    }

    if (cur_byte != -1)
        return EE_FALSE;

    *buf_size = buf_index;
    return !no_buf;
}

int disassemble_and_print(ee_x86_mode_t mode, ee_byte_t* instruction, ee_size_t instruction_size) {

    ee_size_t byte_index = 0;
    ee_uint64_t base_addr = 0;

    switch (mode) {
    case EE_X86_MODE_16:

        base_addr = DISASM_BASE_ADDRESS16;
        break;

    case EE_X86_MODE_32:

        base_addr = DISASM_BASE_ADDRESS32;
        break;

    case EE_X86_MODE_64:

        base_addr = DISASM_BASE_ADDRESS64;
        break;

    default:

        break;
    }

    while (byte_index < instruction_size) {

        ee_x86_disasm_output_t dis_out = { 0 };
        ee_ascii_char_t dis_format[128] = { 0 };
        ee_size_t dis_format_size = EE_GET_ARRAY_LEN(dis_format);

        /* Call ee_x86 decoder. */
        if (!ee_x86_disasm(mode, &instruction[byte_index], instruction_size - byte_index, &dis_out))
            return 1;

        /* Obtain string representation of ee_x86 decoder output. */
        if (!ee_x86_format(mode, base_addr + byte_index, &dis_out, dis_format, &dis_format_size))
            return 1;

        nc_println(dis_format);
        byte_index += dis_out.num_instruction_bytes;
    }

    if (byte_index != instruction_size)
        return 1;

    return 0;
}
