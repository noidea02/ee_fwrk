#pragma once

#include <ctype.h>
#include <stdint.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <filesystem>
#include <fstream>
#include <map>
#include <memory>
#include <optional>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace ee_supp {

    template<typename T, typename... Args>
    constexpr std::array<T, sizeof...(Args)> make_array(Args... args) {
        return { (T)args...};
    }

    template<typename T, size_t S>
    constexpr auto make_array_fill(T val) {

        std::array<T, S> result{};
        std::ranges::fill(result.begin(), result.end(), val);

        return result;
    }

    template<std::integral T, size_t S>
    constexpr auto make_array_incremental_fill(T start = 0) {

        return[&]<size_t... I>(std::index_sequence<I...>) {

            return std::array<T, S>{ static_cast<T>(start + I)... };

        }(std::make_index_sequence<S>());
    }

    template<typename T, size_t S, std::ranges::input_range R>
        requires(sizeof(T) == sizeof(std::ranges::range_value_t<R>))
    constexpr auto make_array_with_n_from_range(R&& r, size_t n) {

        std::array<T, S> result{};
        const auto min_n{ std::min(S, n) };
        const auto clamped_view{ std::forward<R>(r) | std::views::take(min_n) };

        size_t index{};
        for (auto&& val : clamped_view)
            result[index++] = static_cast<T>(val);

        return result;
    }

    template<typename T, size_t N, std::ranges::input_range R>
        requires(sizeof(T) == sizeof(std::ranges::range_value_t<R>))
    constexpr auto make_array_with_n_from_range(R&& r) {

        return make_array_with_n_from_range<T, N>(r, N);
    }

    template<typename T, size_t N0, size_t N1>
    constexpr auto concat_arrays(const std::array<T, N0>& a0, const std::array<T, N1>& a1) {

        return[&]<size_t... I0, size_t... I1>(std::index_sequence<I0...>, std::index_sequence<I1...>) {

            return std::array<T, N0 + N1>{ a0[I0]..., a1[I1]... };

        }(std::make_index_sequence<N0>(), std::make_index_sequence<N1>());
    }

    inline std::optional<uint8_t> hex_char_to_byte(char c) {

        if (c >= '0' && c <= '9')
            return c - '0';

        if (c >= 'a' && c <= 'f')
            return c - 'a' + 10;

        if (c >= 'A' && c <= 'F')
            return c - 'A' + 10;

        return std::nullopt;
    }

    inline bool read_file(const std::filesystem::path& file_path, std::vector<uint8_t>& out) {

        std::ifstream file(file_path.native(), std::ios::binary);
        if (!file)
            return false;

        file.seekg(0, std::ios::end);

        const auto file_size{ file.tellg() };
        if (file_size < 0)
            return false;

        file.seekg(0, std::ios::beg);

        out.resize(file_size);
        if (!file.read((char*)out.data(), file_size))
            return false;

        return true;
    }

    inline bool read_hex_file(const std::filesystem::path& file_path, std::vector<uint8_t>& out) {

        if (!read_file(file_path, out))
            return false;

        size_t out_index{};
        int high_nibble{ -1 };

        for (size_t in_index{}; in_index != out.size(); ++in_index) {

            const auto cur_char{ out.at(in_index) };
            if (cur_char == '\r' || cur_char == '\n')
                continue;

            const auto byte_val{ hex_char_to_byte(cur_char) };
            if (!byte_val)
                return false;

            if (high_nibble == -1) {

                high_nibble = byte_val.value();
                continue;
            }

            out.at(out_index++) = static_cast<uint8_t>((high_nibble << 4) | byte_val.value());
            high_nibble = -1;
        }

        out.resize(out_index);
        return true;
    }

    inline std::string str_to_lower(std::string_view in) {
        
        return in
            | std::views::transform([](unsigned char chr) { 
                    return static_cast<char>(tolower(chr)); 
                })
            | std::ranges::to<std::string>();
    }

    inline std::vector<std::string> tokenize_str(std::string_view str, std::string_view delim) {

        return str
            | std::views::split(delim)
            | std::views::transform([](auto&& subr) {
                    return subr | std::ranges::to<std::string>();
                })
            | std::ranges::to<std::vector<std::string>>();
    }

    template<typename KeyT, typename ValT>
    bool map_has_key(const std::map<KeyT, ValT>& m, KeyT&& key) {
        return m.find(std::forward<KeyT>(key)) != m.end();
    }

    template<typename KeyT, typename ValT>
    ValT map_get_value(const std::map<KeyT, ValT>& m, KeyT&& key) {

        const auto result_it{ m.find(std::forward<KeyT>(key)) };
        if (result_it == m.end())
            std::terminate();

        return result_it->second;
    }

    template<std::ranges::input_range T>
        requires(sizeof(std::ranges::range_value_t<T>) == 1)
    std::string byte_range_to_hex_str(T&& br) {

        std::stringstream strstream;
        for (const auto b : br)
            strstream << std::format("\\x{:02X}", b);

        return strstream.str();
    }
}