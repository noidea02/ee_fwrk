#ifndef NO_CRT_IG
#define NO_CRT_IG

#include <ee/ee_base.h>

#ifdef EE_TARGET_WINDOWS
    #define NC_NO_RETURN __declspec(noreturn)
#else
    #define NC_NO_RETURN __attribute__((noreturn))
#endif

NC_NO_RETURN void nc_trap(void);
void* nc_calloc(ee_size_t num, ee_size_t size);
void nc_free(void* ptr);
void nc_print(const char* str);
void nc_println(const char* ln);
ee_bool_t nc_read_file(const char* path, ee_byte_t** content, ee_size_t* size);

#endif
