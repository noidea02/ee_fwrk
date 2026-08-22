#include <Windows.h>

#include "no_crt.h"

static LPSTR convert_wstr_to_str(LPWSTR wstr) {

    int buf_size = 0;
    int conv_rv = 0;
    LPSTR out = NULL;

    buf_size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if (buf_size <= 0)
        nc_trap();

    out = nc_calloc(buf_size, 1);
    conv_rv = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, out, buf_size, NULL, NULL);

    if (!conv_rv)
        nc_trap();

    return out;
}

static void get_cmdline(int* argc, char*** argv) {
    
    LPWSTR* argv_w = NULL;
    int tmp_argc = 0;
    char** tmp_argv = NULL;
    ee_size_t arg_index = 0;

    argv_w = CommandLineToArgvW(GetCommandLineW(), &tmp_argc);
    if (!argv_w)
        nc_trap();

    tmp_argv = nc_calloc(tmp_argc, sizeof(char*));
    for (; arg_index != tmp_argc; ++arg_index) {

        const LPWSTR arg_w = argv_w[arg_index];
        const LPSTR arg = convert_wstr_to_str(arg_w);

        tmp_argv[arg_index] = arg;
    }

    *argc = tmp_argc;
    *argv = tmp_argv;

    LocalFree(argv_w);
}

NC_NO_RETURN void nc_trap(void) {
    DebugBreak();
}

int wrapped_main(int argc, char** argv);
void __stdcall nc_main(void) {

    int argc = 0;
    char** argv = NULL;
    int exit_code = 0;

    get_cmdline(&argc, &argv);
    exit_code = wrapped_main(argc, argv);

    if (argv) {

        ee_size_t arg_index = 0;
        for (; arg_index != (ee_size_t)argc; ++arg_index) {

            char* const arg = argv[arg_index];
            nc_free(arg);
        }

        nc_free(argv);
    }

    ExitProcess(exit_code);
}

void* nc_calloc(ee_size_t num, ee_size_t size) {

    num = num ? num : 1;
    size = size ? size : 1;

    if (num > (EE_SIZE_MAX / size))
        nc_trap();

    return HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY, (SIZE_T)(num * size));
}

void nc_free(void* ptr) {

    if (!HeapFree(GetProcessHeap(), 0, ptr))
        nc_trap();
}

void nc_print(const char* str) {

    HANDLE stdout_h = NULL;
    DWORD bytes_written = 0;

    stdout_h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (stdout_h == INVALID_HANDLE_VALUE || !stdout_h)
        nc_trap();

    WriteFile(stdout_h, str, ee_strlen(str), &bytes_written, NULL);
}

void nc_println(const char* ln) {

    ee_char8_t actual_ln[1024] = { 0 };
    if (!ee_strapp(actual_ln, sizeof(actual_ln), ln) || !ee_strapp(actual_ln, sizeof(actual_ln), "\r\n"))
        nc_trap();

    nc_print(actual_ln);
}

ee_bool_t nc_read_file(const char* path, ee_byte_t** content, ee_size_t* size) {

    HANDLE file_h = NULL;
    LARGE_INTEGER wrapped_size = { 0 };
    ee_size_t tmp_size = 0;
    ee_byte_t* tmp_content = NULL;
    DWORD num_bytes_read = 0;
    ee_bool_t success = EE_FALSE;
    
    file_h = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (file_h == INVALID_HANDLE_VALUE)
        goto cleanup;

    if (!GetFileSizeEx(file_h, &wrapped_size))
        goto cleanup;

    tmp_size = (ee_size_t)wrapped_size.QuadPart;
    tmp_content = nc_calloc(tmp_size, 1);

    if (!ReadFile(file_h, tmp_content, tmp_size, &num_bytes_read, NULL) || num_bytes_read != tmp_size)
        goto cleanup;

    *content = tmp_content;
    *size = tmp_size;
    success = EE_TRUE;

cleanup:

    if (file_h && file_h != INVALID_HANDLE_VALUE)
        CloseHandle(file_h);

    if (!success && tmp_content)
        nc_free(tmp_content);

    return success;
}
