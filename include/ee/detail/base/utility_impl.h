#ifndef EE_BASE_UTILITY_IMPL_IG
#define EE_BASE_UTILITY_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

ee_int32_t ee_memcmp(const void* buf0, const void* buf1, ee_size_t count) {

    const ee_byte_t* const buf0_bytes = (const ee_byte_t*)buf0;
    const ee_byte_t* const buf1_bytes = (const ee_byte_t*)buf1;
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

void* ee_memcpy(void* dst, const void* src, ee_size_t count) {

    ee_byte_t* const dst_bytes = (ee_byte_t*)dst;
    const ee_byte_t* const src_bytes = (const ee_byte_t*)src;
    ee_size_t index = 0;

    for (index = 0; index != count; ++index)
        dst_bytes[index] = src_bytes[index];

    return dst;
}

void* ee_memmove(void* dst, const void* src, ee_size_t count) {
    
    ee_byte_t* const dst_bytes = (ee_byte_t*)dst;
    const ee_byte_t* const src_bytes = (ee_byte_t*)src;

    if (dst == src || !count)
        return dst;
    
    if ((ee_size_t)dst_bytes < (ee_size_t)src_bytes) {

        ee_size_t index = 0;
        for (; index != count; ++index)
            dst_bytes[index] = src_bytes[index];
    }
    else {

        ee_size_t index = count;
        for (; index != 0; --index)
            dst_bytes[index - 1] = src_bytes[index - 1];
    }

    return dst;
}

void* ee_memset(void* dst, ee_byte_t val, ee_size_t count) {

    ee_byte_t* const bytes = (ee_byte_t*)dst;
    ee_size_t index = 0;

    for (index = 0; index != count; ++index)
        bytes[index] = val;

    return dst;
}

ee_size_t ee_strlen(const ee_char8_t* str) {

    ee_size_t len = 0;

    while (str[len])
        ++len;

    return len;
}

ee_char8_t* ee_strncpy(ee_char8_t* dst, const ee_char8_t* src, ee_size_t dst_buf_size) {

    ee_size_t chr_index = 0;

    if (!dst_buf_size)
        return dst;

    for (; chr_index != (dst_buf_size - 1) && src[chr_index]; ++chr_index)
        dst[chr_index] = src[chr_index];

    dst[chr_index] = 0;
    return dst;
}

ee_int32_t ee_strcmp(const ee_char8_t* str1, const ee_char8_t* str2) {

    const ee_size_t str1_len = ee_strlen(str1);
    const ee_size_t str2_len = ee_strlen(str2);

    if (str1_len < str2_len)
        return -1;

    if (str1_len > str2_len)
        return 1;

    return ee_memcmp(str1, str2, str1_len);
}

ee_bool_t ee_strapp(ee_char8_t* dst, ee_size_t dst_buf_size, const ee_char8_t* src) {

    ee_size_t null_term_index = 0;
    const ee_size_t src_len = ee_strlen(src) + 1;

    if (!dst_buf_size)
        return EE_FALSE;

    if (src_len == 1)
        return EE_TRUE;

    null_term_index = ee_strlen(dst);
    if (null_term_index + src_len > dst_buf_size)
        return EE_FALSE;

    ee_memcpy(&dst[null_term_index], src, src_len);
    return EE_TRUE;
}

ee_bool_t ee_strpad(const ee_char8_t* src, ee_size_t desired_size, ee_bool_t pad_front, ee_char8_t pad_chr, ee_char8_t* dst, ee_size_t dst_buf_size) {

    const ee_size_t src_len = ee_strlen(src);

    if (desired_size >= dst_buf_size)
        return EE_FALSE;

    if (src_len == desired_size) {

        /* Scenario 1: Just copy as-is. */
        ee_memcpy(dst, src, src_len + 1);
    }
    else {

        /* Scneario 2: Pad or trim. */
        const ee_bool_t do_pad = src_len < desired_size;
        if (do_pad) {

            const ee_size_t dst_index = pad_front ? (desired_size - src_len) : 0;

            ee_memset(dst, pad_chr, desired_size);
            ee_memcpy(&dst[dst_index], src, src_len);
        }
        else {

            const ee_size_t src_index = pad_front ? (src_len - desired_size) : 0;
            ee_memcpy(dst, &src[src_index], desired_size);
        }

        dst[desired_size] = 0;
    }

    return EE_TRUE;
}

ee_bool_t ee_itoa32(ee_int32_t in, ee_char8_t* out, ee_size_t out_size, ee_bool_t hex) {

    ee_char8_t tmp_out[22] = { 0 };
    ee_size_t src_index = 0;
    ee_size_t total_src_len = 0;

    if (!ee_itoa64(in, tmp_out, sizeof(tmp_out), hex))
        return EE_FALSE;

    if (in < 0 && hex) {

        const ee_size_t tmp_out_len = ee_strlen(tmp_out);
        if (tmp_out_len % 2)
            return EE_FALSE;

        src_index = tmp_out_len / 2;
        total_src_len = src_index + 1;
    }
    else {

        src_index = 0;
        total_src_len = ee_strlen(tmp_out) + 1;
    }

    if (total_src_len > out_size)
        return EE_FALSE;

    ee_memcpy(out, &tmp_out[src_index], total_src_len);
    return EE_TRUE;
}

ee_bool_t ee_itoa64(ee_int64_t in, ee_char8_t* out, ee_size_t out_size, ee_bool_t hex) {

    const ee_char8_t digit_map[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

    ee_uint64_t remaining_digits = (ee_uint64_t)((in < 0 && !hex) ? -in : in);
    const ee_uint64_t radix = hex ? 16 : 10;
    ee_char8_t tmp_out[22] = { 0 };
    ee_int32_t tmp_out_index = 0;
    ee_size_t out_index = 0;

    do {

        const ee_uint64_t digit = (remaining_digits % radix);
        remaining_digits /= radix;

        if (digit >= sizeof(digit_map) || tmp_out_index == sizeof(tmp_out) - 1)
            return EE_FALSE;

        tmp_out[tmp_out_index] = digit_map[digit];
        ++tmp_out_index;

    } while (remaining_digits);

    if (!hex && in < 0) {

        if (tmp_out_index == sizeof(tmp_out) - 1)
            return EE_FALSE;

        tmp_out[tmp_out_index] = '-';
        ++tmp_out_index;
    }
    else if (hex && tmp_out_index % 2) {

        tmp_out[tmp_out_index] = '0';
        ++tmp_out_index;
    }

    if ((ee_size_t)tmp_out_index >= out_size)
        return EE_FALSE;

    --tmp_out_index;
    for (; tmp_out_index > -1; --tmp_out_index) {

        out[out_index] = tmp_out[tmp_out_index];
        ++out_index;
    }

    out[out_index] = 0;
    return EE_TRUE;
}

#endif
