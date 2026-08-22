#pragma once

#include <bit>
#include <concepts>
#include <limits>

#include <gtest/gtest.h>

namespace ee_test {

    template<typename F, typename ArgRetT>
    concept UnaryFunction = requires(F && f, ArgRetT arg) {
        { f(arg) } -> std::convertible_to<ArgRetT>;
    };

    template<std::endian NoChangeEndianV, typename ValT, typename ConversionParamT, UnaryFunction<ConversionParamT> ConversionF>
    void test_base_endian_conversion(ConversionF func, ValT val) {

        if constexpr (std::endian::native == NoChangeEndianV)
            EXPECT_EQ(func((ConversionParamT)val), (ConversionParamT)val);
        else
            EXPECT_EQ(func((ConversionParamT)val), (ConversionParamT)std::byteswap(val));
    }

    template<std::endian NoChangeEndianV, typename ValT, UnaryFunction<ValT> ConversionF>
    void test_base_endian_conversion(ConversionF func, ValT val) {

        test_base_endian_conversion<NoChangeEndianV, ValT, ValT, ConversionF>(func, val);
    }
}