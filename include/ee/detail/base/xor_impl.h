#ifndef EE_BASE_XOR_IMPL_IG
#define EE_BASE_XOR_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

void ee_xor(const ee_byte_t* in, ee_size_t in_len, const ee_byte_t* key, ee_size_t key_len, ee_byte_t* out, ee_size_t out_len) {

    const ee_size_t byte_count = EE_MIN(in_len, out_len);
    ee_size_t byte_index = 0;
    ee_size_t key_byte_index = 0;

    for (; byte_index != byte_count; ++byte_index) {

        ee_byte_t key_byte = 0;
        if (key_len) {

            key_byte = key[key_byte_index++];

            if (key_byte_index == key_len)
                key_byte_index = 0;
        }

        out[byte_index] = in[byte_index] ^ key_byte;
    }
}

#endif
