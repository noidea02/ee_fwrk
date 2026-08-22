#ifndef EE_BASE_IG
#define EE_BASE_IG

/* Environment detection macros
*/

#ifdef _WIN32
    #define EE_TARGET_WINDOWS
#elif defined(__linux__)
    #define EE_TARGET_LINUX
#elif defined(__APPLE__)
    #define EE_TARGET_APPLE
#else
    #error Unknown target platform.
#endif

#ifdef EE_TARGET_WINDOWS
    #ifdef _WIN64
        #define EE_TARGET_64BIT
    #elif defined(_WIN32)
        #define EE_TARGET_32BIT
    #endif
#else
    #ifndef __SIZEOF_POINTER__
        #error Unable to determine pointer width.
    #elif __SIZEOF_POINTER__ == 4
        #define EE_TARGET_32BIT
    #elif __SIZEOF_POINTER__ == 8
        #define EE_TARGET_64BIT
    #else
        #error Unexpected pointer width.
    #endif
#endif

/* Types
*/

typedef char ee_int8_t;
typedef short ee_int16_t;
typedef int ee_int32_t;
typedef unsigned char ee_uint8_t;
typedef unsigned short ee_uint16_t;
typedef unsigned int ee_uint32_t;

#ifdef EE_TARGET_WINDOWS
typedef long long ee_int64_t;
typedef unsigned long long ee_uint64_t;
#else
typedef long ee_int64_t;
typedef unsigned long ee_uint64_t;
#endif

typedef ee_uint8_t ee_byte_t;
typedef ee_byte_t ee_bool_t;
typedef ee_int8_t ee_char8_t;
typedef ee_char8_t ee_ascii_char_t;

#ifdef EE_TARGET_64BIT
typedef ee_uint64_t ee_size_t;
#else
typedef ee_uint32_t ee_size_t;
#endif

/* Constants
*/

#define EE_FALSE ((ee_bool_t)0)
#define EE_TRUE ((ee_bool_t)1)

#define EE_INT8_MAX ((ee_int8_t)0x7f)
#define EE_INT16_MAX ((ee_int16_t)0x7fff)
#define EE_INT32_MAX ((ee_int32_t)0x7fffffff)
#define EE_INT64_MAX ((ee_int64_t)0x7fffffffffffffff)
#define EE_UINT8_MAX ((ee_uint8_t)0xff)
#define EE_UINT16_MAX ((ee_uint16_t)0xffff)
#define EE_UINT32_MAX ((ee_uint32_t)0xffffffff)
#define EE_UINT64_MAX ((ee_uint64_t)0xffffffffffffffff)
#define EE_BYTE_MAX ((ee_byte_t)EE_UINT8_MAX)

#ifdef EE_TARGET_64BIT
    #define EE_SIZE_MAX EE_UINT64_MAX
#else
    #define EE_SIZE_MAX EE_UINT32_MAX
#endif

/* Callback types
*/

typedef void* (*ee_calloc_t)(ee_size_t num, ee_size_t size);
typedef void (*ee_free_t)(void* ptr);

typedef ee_bool_t(*ee_read_data_t)(void* ctx, ee_uint32_t offset, ee_size_t size, void* out);

typedef struct {

    void* ctx;
    ee_read_data_t cb;

} ee_read_data_wrapper_t;

/* General utility functions
*/

#define EE_GET_ARRAY_LEN(arr) (sizeof(arr) / sizeof(arr[0]))
#define EE_GET_TDEF_STRUCT_FIELD_LEN(s, f) sizeof(((s*)0)->f)

#define EE_MIN(a, b) ((a > b) ? b : a)
#define EE_MAX(a, b) ((a > b) ? a : b)

ee_int32_t ee_memcmp(const void* buf0, const void* buf1, ee_size_t count);
void* ee_memcpy(void* dst, const void* src, ee_size_t count);
void* ee_memmove(void* dst, const void* src, ee_size_t count);
void* ee_memset(void* dst, ee_byte_t val, ee_size_t count);
ee_size_t ee_strlen(const ee_char8_t* str);
ee_char8_t* ee_strncpy(ee_char8_t* dst, const ee_char8_t* src, ee_size_t num);
ee_int32_t ee_strcmp(const ee_char8_t* str1, const ee_char8_t* str2);
ee_bool_t ee_strapp(ee_char8_t* dst, ee_size_t dst_buf_size, const ee_char8_t* src);
ee_bool_t ee_strpad(const ee_char8_t* src, ee_size_t desired_size, ee_bool_t pad_front, ee_char8_t pad_chr, ee_char8_t* dst, ee_size_t dst_buf_size);
ee_bool_t ee_itoa32(ee_int32_t in, ee_char8_t* out, ee_size_t out_size, ee_bool_t hex);
ee_bool_t ee_itoa64(ee_int64_t in, ee_char8_t* out, ee_size_t out_size, ee_bool_t hex);

/* Endianess conversion functions
*/

ee_bool_t ee_endian_platform_is_le(void);
ee_int16_t ee_endian_le_to_native_int16(ee_int16_t in);
ee_int32_t ee_endian_le_to_native_int32(ee_int32_t in);
ee_int64_t ee_endian_le_to_native_int64(ee_int64_t in);
ee_int16_t ee_endian_be_to_native_int16(ee_int16_t in);
ee_int32_t ee_endian_be_to_native_int32(ee_int32_t in);
ee_int64_t ee_endian_be_to_native_int64(ee_int64_t in);
ee_int16_t ee_endian_native_to_le_int16(ee_int16_t in);
ee_int32_t ee_endian_native_to_le_int32(ee_int32_t in);
ee_int64_t ee_endian_native_to_le_int64(ee_int64_t in);
ee_int16_t ee_endian_native_to_be_int16(ee_int16_t in);
ee_int32_t ee_endian_native_to_be_int32(ee_int32_t in);
ee_int64_t ee_endian_native_to_be_int64(ee_int64_t in);

/* Obfuscation functions
*/

#define EE_SL(sl) sl
#define EE_OBFUSCATED_SL(in, out, out_size) ee_deobfuscate_str((const ee_byte_t*)in, sizeof(in) - 1, out, out_size)

ee_byte_t* ee_obfuscate_str(const ee_char8_t* in, ee_byte_t* out, ee_size_t out_size);
ee_char8_t* ee_deobfuscate_str(const ee_byte_t* in, ee_size_t in_size, ee_char8_t* out, ee_size_t out_size);

/* XOR encryption functions
*/

void ee_xor(const ee_byte_t* in, ee_size_t in_len, const ee_byte_t* key, ee_size_t key_len, ee_byte_t* out, ee_size_t out_len);

/* Static assertions
*/

#define EE_STATIC_ASSERT(cond, msg) typedef char ee_static_assert_##msg[(cond) ? 1 : -1]

EE_STATIC_ASSERT(sizeof(ee_int8_t) == 1, ee_int8_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_int16_t) == 2, ee_int16_t_must_be_2_bytes);
EE_STATIC_ASSERT(sizeof(ee_int32_t) == 4, ee_int32_t_must_be_4_bytes);
EE_STATIC_ASSERT(sizeof(ee_int64_t) == 8, ee_int64_t_must_be_8_bytes);
EE_STATIC_ASSERT(sizeof(ee_uint8_t) == 1, ee_uint8_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_uint16_t) == 2, ee_uint16_t_must_be_2_bytes);
EE_STATIC_ASSERT(sizeof(ee_uint32_t) == 4, ee_uint32_t_must_be_4_bytes);
EE_STATIC_ASSERT(sizeof(ee_uint64_t) == 8, ee_uint64_t_must_be_8_bytes);
EE_STATIC_ASSERT(sizeof(ee_byte_t) == 1, ee_byte_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_bool_t) == 1, ee_bool_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_char8_t) == 1, ee_char8_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_ascii_char_t) == 1, ee_ascii_char_t_must_be_1_bytes);
EE_STATIC_ASSERT(sizeof(ee_size_t) == sizeof(void*), ee_size_t_must_match_ptr_size);

EE_STATIC_ASSERT(EE_FALSE == 0, ee_false_must_be_0);
EE_STATIC_ASSERT(EE_TRUE == 1, ee_true_must_be_1);
EE_STATIC_ASSERT(EE_INT8_MAX == 0x7f, ee_int8_max_must_be_0x7f);
EE_STATIC_ASSERT(EE_INT16_MAX == 0x7fff, ee_int16_max_must_be_0x7fff);
EE_STATIC_ASSERT(EE_INT32_MAX == 0x7fffffff, ee_int32_max_must_be_0x7fffffff);
EE_STATIC_ASSERT(EE_INT64_MAX == 0x7fffffffffffffff, ee_int64_max_must_be_0x7fffffffffffffff);
EE_STATIC_ASSERT(EE_UINT8_MAX == 0xff, ee_uint8_max_must_be_0xff);
EE_STATIC_ASSERT(EE_UINT16_MAX == 0xffff, ee_uint16_max_must_be_0xffff);
EE_STATIC_ASSERT(EE_UINT32_MAX == 0xffffffff, ee_uint32_max_must_be_0xffffffff);
EE_STATIC_ASSERT(EE_UINT64_MAX == 0xffffffffffffffff, ee_uint64_max_must_be_0xffffffffffffffff);
EE_STATIC_ASSERT(EE_BYTE_MAX == 0xff, ee_byte_max_must_be_0xff);

#ifdef EE_TARGET_64BIT
EE_STATIC_ASSERT(EE_SIZE_MAX == EE_UINT64_MAX, ee_size_max_must_match_ee_uint64_max);
#else
EE_STATIC_ASSERT(EE_SIZE_MAX == EE_UINT32_MAX, ee_size_max_must_match_ee_uint32_max);
#endif

EE_STATIC_ASSERT(EE_MIN(0, 0) == 0, ee_min_0_0_must_be_0);
EE_STATIC_ASSERT(EE_MIN(0, 1) == 0, ee_min_0_1_must_be_0);
EE_STATIC_ASSERT(EE_MIN(1, 0) == 0, ee_min_1_0_must_be_0);
EE_STATIC_ASSERT(EE_MAX(0, 0) == 0, ee_max_0_0_must_be_0);
EE_STATIC_ASSERT(EE_MAX(0, 1) == 1, ee_max_0_1_must_be_1);
EE_STATIC_ASSERT(EE_MAX(1, 0) == 1, ee_max_1_0_must_be_1);

/* Implementation
*/

#ifdef EE_BASE_IMPL
    #define EE_PRV_UNLOCK_DETAIL
    #include "ee/detail/base/endian_impl.h"
    #include "ee/detail/base/obfuscate_impl.h"
    #include "ee/detail/base/utility_impl.h"
    #include "ee/detail/base/xor_impl.h"
    #undef EE_PRV_UNLOCK_DETAIL
#endif

#endif
