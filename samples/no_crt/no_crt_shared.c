#include <ee/ee_base.h>

/* Implementation of fundamentals taken from ee_base and modified to prevent optimization (thus avoiding endless recursive calls).
*/

int memcmp(const void* buf0, const void* buf1, ee_size_t count) {
    
    const volatile ee_byte_t* const buf0_bytes = (const ee_byte_t*)buf0;
    const volatile ee_byte_t* const buf1_bytes = (const ee_byte_t*)buf1;
    ee_size_t index = 0;
    ee_int32_t magnitude = 0;

    if (buf0 == buf1)
        return 0;

    for (index = 0; !magnitude && index != count; ++index) {

        if (buf0_bytes[index] < buf1_bytes[index])
            magnitude = -1;
        else if (buf0_bytes[index] > buf1_bytes[index])
            magnitude = 1;
    }

    return magnitude;
}

void* memcpy(void* dst, const void* src, ee_size_t count) {
    
    volatile ee_byte_t* const dst_bytes = (ee_byte_t*)dst;
    const volatile ee_byte_t* const src_bytes = (const ee_byte_t*)src;
    ee_size_t index = 0;

    for (index = 0; index != count; ++index)
        dst_bytes[index] = src_bytes[index];

    return dst;
}

void* memmove(void* dst, const void* src, ee_size_t num) {
    
    volatile ee_byte_t* const dst_bytes = (ee_byte_t*)dst;
    const volatile ee_byte_t* const src_bytes = (ee_byte_t*)src;

    if (dst == src || !num)
        return dst;

    if ((ee_size_t)dst_bytes < (ee_size_t)src_bytes) {

        ee_size_t index = 0;
        for (; index != num; ++index)
            dst_bytes[index] = src_bytes[index];
    }
    else {

        ee_size_t index = num;
        for (; index != 0; --index)
            dst_bytes[index - 1] = src_bytes[index - 1];
    }

    return dst;
}

void* memset(void* dst, int val, ee_size_t count) {
    
    volatile ee_byte_t* const bytes = (ee_byte_t*)dst;
    ee_size_t index = 0;

    for (index = 0; index != count; ++index)
        bytes[index] = (ee_byte_t)val;

    return dst;
}
