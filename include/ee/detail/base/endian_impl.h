#ifndef EE_BASE_ENDIAN_IMPL_IG
#define EE_BASE_ENDIAN_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

#define EE_PRV_ENDIAN_DEFINE_FLIP_INT_FUNC(bits)\
static ee_int##bits##_t ee_prv_endian_flip_int##bits(ee_int##bits##_t in) {\
    \
    ee_int##bits##_t out = 0;\
    const ee_byte_t* const in_bytes = (const ee_byte_t*)&in;\
    ee_byte_t* const out_bytes = (ee_byte_t*)&out;\
    ee_size_t byte_index = 0;\
    \
    for(; byte_index != sizeof(out); ++byte_index)\
        out_bytes[byte_index] = in_bytes[sizeof(out) - 1 - byte_index];\
        \
    return out;\
}

EE_PRV_ENDIAN_DEFINE_FLIP_INT_FUNC(16)
EE_PRV_ENDIAN_DEFINE_FLIP_INT_FUNC(32)
EE_PRV_ENDIAN_DEFINE_FLIP_INT_FUNC(64)

ee_bool_t ee_endian_platform_is_le(void) {

    const ee_int16_t native = 0x1122;
    const ee_byte_t* const native_bytes = (const ee_byte_t*)&native;

    return native_bytes[0] == 0x22;
}

ee_int16_t ee_endian_le_to_native_int16(ee_int16_t in) {

    if (ee_endian_platform_is_le())
        return in;
    else
        return ee_prv_endian_flip_int16(in);
}

ee_int32_t ee_endian_le_to_native_int32(ee_int32_t in) {

    if (ee_endian_platform_is_le())
        return in;
    else
        return ee_prv_endian_flip_int32(in);
}

ee_int64_t ee_endian_le_to_native_int64(ee_int64_t in) {

    if (ee_endian_platform_is_le())
        return in;
    else
        return ee_prv_endian_flip_int64(in);
}

ee_int16_t ee_endian_be_to_native_int16(ee_int16_t in) {

    if (ee_endian_platform_is_le())
        return ee_prv_endian_flip_int16(in);
    else
        return in;
}

ee_int32_t ee_endian_be_to_native_int32(ee_int32_t in) {

    if (ee_endian_platform_is_le())
        return ee_prv_endian_flip_int32(in);
    else
        return in;
}

ee_int64_t ee_endian_be_to_native_int64(ee_int64_t in) {

    if (ee_endian_platform_is_le())
        return ee_prv_endian_flip_int64(in);
    else
        return in;
}

ee_int16_t ee_endian_native_to_le_int16(ee_int16_t in) {

    return ee_endian_le_to_native_int16(in);
}

ee_int32_t ee_endian_native_to_le_int32(ee_int32_t in) {

    return ee_endian_le_to_native_int32(in);
}

ee_int64_t ee_endian_native_to_le_int64(ee_int64_t in) {

    return ee_endian_le_to_native_int64(in);
}

ee_int16_t ee_endian_native_to_be_int16(ee_int16_t in) {

    return ee_endian_be_to_native_int16(in);
}

ee_int32_t ee_endian_native_to_be_int32(ee_int32_t in) {

    return ee_endian_be_to_native_int32(in);
}

ee_int64_t ee_endian_native_to_be_int64(ee_int64_t in) {

    return ee_endian_be_to_native_int64(in);
}

#endif
