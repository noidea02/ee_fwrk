#ifndef EE_BASE_OBFUSCATE_IMPL_IG
#define EE_BASE_OBFUSCATE_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

/* WARNING: Changing the obfuscation key without adjusting the obfuscated strings (EE_OBFUSCATED_SL) will break the library.
*/

#define EE_OBFUSCATION_KEY "\xCC\xCC\x55\x54\xE8\x04\x2A\x41\x00\x8B\x00\x5C\x5D\xC3\xCC\xCC"
#define EE_OBFUSCATION_KEY_LEN (sizeof(EE_OBFUSCATION_KEY) - 1)

ee_byte_t* ee_obfuscate_str(const ee_char8_t* in, ee_byte_t* out, ee_size_t out_size) {

    if (!out_size)
        return out;

    ee_xor((ee_byte_t*)in, ee_strlen(in), (const ee_byte_t*)EE_OBFUSCATION_KEY, EE_OBFUSCATION_KEY_LEN, out, out_size);
    return out;
}

ee_char8_t* ee_deobfuscate_str(const ee_byte_t* in, ee_size_t in_size, ee_char8_t* out, ee_size_t out_size) {

    if (!out_size)
        return out;

    ee_memset(out, 0, out_size);
    if (out_size == 1)
        return out;

    ee_xor(in, in_size, (const ee_byte_t*)EE_OBFUSCATION_KEY, EE_OBFUSCATION_KEY_LEN, (ee_byte_t*)out, out_size - 1);
    return out;
}

#endif
