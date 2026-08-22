/*

Converts an Intel VEX prefix specification string into VEX prefix bytes (either 2 or 3 bytes). The prefix specification string
must adhere to the following format:

VEX.[128,256].[66,F2,F3].0F/0F3A/0F38.[W0,W1]

One of the following arguments MUST precede the specification string:

-32 : Operate in x86_32 mode.
-64 : Operate in x86_64 mode.

Any of the following arguments CAN precede the specification string:

-force2b : Produce the 2-byte prefix variant and return an error code (not 0) if this is not possible. 
-force3b : Produce the 3-byte prefix variant.
-rxb=000 : REX.RXB bits (non-inverted form).
-vvvv=0000 : VEX.vvvv bits (non-inverted form).

Generally, if contradictory arguments are specified, an error code is returned.

*/

#include <stdint.h>

#include <charconv>
#include <iostream>
#include <map>
#include <optional>
#include <ranges>
#include <string>
#include <vector>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace {

    enum class x86_mode_t {
        X86_32,
        X86_64
    };

    enum class vex_prefix_size_t {
        SIZE_2B,
        SIZE_3B
    };

    struct cmdline_t {

        x86_mode_t x86_mode;
        std::optional<vex_prefix_size_t> opt_force_prefix_size;
        std::string vex_spec_str;
        std::optional<uint8_t> opt_rex_rxb_bits;
        std::optional<uint8_t> opt_vex_vvvv_bits;

        cmdline_t() : cmdline_t(x86_mode_t::X86_32, std::nullopt, "", std::nullopt, std::nullopt) {}

        cmdline_t(x86_mode_t x86_mode, const std::optional<vex_prefix_size_t>& opt_force_prefix_size,
            std::string_view vex_spec_str, const std::optional<uint8_t>& opt_rex_rxb_bits, const std::optional<uint8_t>& opt_vex_vvvv_bits) :
            x86_mode{ x86_mode }, opt_force_prefix_size{ opt_force_prefix_size }, vex_spec_str{ vex_spec_str }, opt_rex_rxb_bits{ opt_rex_rxb_bits },
            opt_vex_vvvv_bits{ opt_vex_vvvv_bits } {}
    };

    struct vex_prefix_spec_t {

        std::optional<uint8_t> opt_l_bit;
        std::optional<uint8_t> opt_pp_bits;
        uint8_t mmmmm_bits;
        std::optional<uint8_t> opt_rex_w_bit;

        vex_prefix_spec_t() : vex_prefix_spec_t(std::nullopt, std::nullopt, 0, std::nullopt) {}

        vex_prefix_spec_t(const std::optional<uint8_t>& opt_l_bit, const std::optional<uint8_t>& opt_pp_bits, uint8_t mmmmm_bits,
            const std::optional<uint8_t>& opt_rex_w_bit) : opt_l_bit{ opt_l_bit }, opt_pp_bits{ opt_pp_bits }, mmmmm_bits{ mmmmm_bits },
            opt_rex_w_bit{ opt_rex_w_bit } {}
    };

    bool parse_cmdline(const std::vector<std::string>& tokens, cmdline_t& result);
    bool parse_vex_spec_str(std::string_view str, vex_prefix_spec_t& result);
    bool build_vex_prefix_vec(const cmdline_t& cmdline, const vex_prefix_spec_t& spec, std::vector<uint8_t>& result);
}

int main(int argc, char* argv[]) {

    std::vector<std::string> args(argv, argv + argc);
    if (args.empty())
        std::terminate();

    args.erase(args.begin());

    cmdline_t cmdline{};
    if (!parse_cmdline(args, cmdline))
        return 1;

    vex_prefix_spec_t prefix_spec{};
    if (!parse_vex_spec_str(cmdline.vex_spec_str, prefix_spec))
        return 1;

    std::vector<uint8_t> prefix_bytes;
    if (!build_vex_prefix_vec(cmdline, prefix_spec, prefix_bytes))
        return 1;

    const auto result{ byte_range_to_hex_str(prefix_bytes) };
    std::cout << result << std::endl;

    return 0;
}

namespace {

    constexpr uint8_t VEX_MMMMM_0F{ 0b01 };
    constexpr uint8_t VEX_MMMMM_0F38{ 0b10 };
    constexpr uint8_t VEX_MMMMM_0F3A{ 0b11 };

    constexpr uint8_t VEX_PP_NONE{ 0b00 };
    constexpr uint8_t VEX_PP_66{ 0b01 };
    constexpr uint8_t VEX_PP_F2{ 0b11 };
    constexpr uint8_t VEX_PP_F3{ 0b10 };

    std::optional<uint8_t> bit_str_to_byte(std::string_view str, size_t num_bits) {

        if (str.size() != num_bits)
            return std::nullopt;

        uint8_t result{};
        const auto[ptr, ec]{ std::from_chars(str.data(), str.data() + str.size(), result, 2) };

        if (ec != std::errc{})
            return std::nullopt;

        uint8_t mask{};
        for (size_t bi{}; bi != num_bits; ++bi)
            mask = (mask << 1) + 1;

        return result & mask;
    }

    bool parse_cmdline(const std::vector<std::string>& args, cmdline_t& result) {

        std::optional<x86_mode_t> opt_mode;
        std::optional<vex_prefix_size_t> opt_force_prefix_size;
        std::optional<std::string> opt_spec_str{};
        std::optional<uint8_t> opt_rex_rxb_bits{};
        std::optional<uint8_t> opt_vex_vvvv_bits{};

        for (size_t arg_index{}; arg_index != args.size(); ++arg_index) {

            const std::string& arg{ args[arg_index] };
            if (!arg.starts_with("-")) {

                if (!arg.starts_with("VEX."))
                    return false;

                opt_spec_str = arg;
                break;
            }

            if (arg == "-32") {

                opt_mode = x86_mode_t::X86_32;
                continue;
            }

            if (arg == "-64") {

                opt_mode = x86_mode_t::X86_64;
                continue;
            }

            if (arg == "-force2b") {

                opt_force_prefix_size = vex_prefix_size_t::SIZE_2B;
                continue;
            }

            if (arg == "-force3b") {

                opt_force_prefix_size = vex_prefix_size_t::SIZE_3B;
                continue;
            }

            if (arg.starts_with("-rxb=")) {

                const auto tokens{ tokenize_str(arg, "=") };
                if (tokens.size() != 2)
                    return false;

                opt_rex_rxb_bits = bit_str_to_byte(tokens.at(1), 3);
                if (!opt_rex_rxb_bits)
                    return false;

                continue;
            }

            if (arg.starts_with("-vvvv=")) {

                const auto tokens{ tokenize_str(arg, "=") };
                if (tokens.size() != 2)
                    return false;

                opt_vex_vvvv_bits = bit_str_to_byte(tokens.at(1), 4);
                if (!opt_vex_vvvv_bits)
                    return false;

                continue;
            }
        }

        if (!opt_mode || !opt_spec_str)
            return false;

        result = cmdline_t(opt_mode.value(), opt_force_prefix_size, opt_spec_str.value(), opt_rex_rxb_bits, opt_vex_vvvv_bits);
        return true;
    }

    bool parse_vex_spec_str(std::string_view str, vex_prefix_spec_t& result) {

        static const std::map<std::string_view, uint8_t> vex_l_mapping{
            { "128", 0 },
            { "256", 1 }
        };

        static const std::map<std::string_view, uint8_t> vex_pp_mapping{
            { "66", 0b01 },
            { "f2", 0b11 },
            { "f3", 0b10 }
        };

        static const std::map<std::string_view, uint8_t> vex_mmmmm_mapping{
            { "0f", 0b01 },
            { "0f38", 0b10 },
            { "0f3a", 0b11 }
        };

        static const std::map<std::string_view, uint8_t> vex_rex_w_mapping{
            { "w0", 0 },
            { "w1", 1 }
        };

        const auto str_lower{ str_to_lower(str) };
        auto tokens{ tokenize_str(str_lower, ".") };

        if (tokens.at(0) != "vex")
            return false;

        tokens.erase(tokens.begin());

        std::optional<uint8_t> opt_l_bit;
        std::optional<uint8_t> opt_pp_bits;
        std::optional<uint8_t> opt_mmmmm_bits;
        std::optional<uint8_t> opt_rex_w_bit;

        for (const auto& token : tokens) {

            if (map_has_key<std::string_view, uint8_t>(vex_l_mapping, token)) {

                if (opt_l_bit)
                    return false;

                opt_l_bit = map_get_value<std::string_view>(vex_l_mapping, token);
                continue;
            }

            if (map_has_key<std::string_view, uint8_t>(vex_pp_mapping, token)) {

                if (opt_pp_bits)
                    return false;

                opt_pp_bits = map_get_value<std::string_view>(vex_pp_mapping, token);
                continue;
            }

            if (map_has_key<std::string_view, uint8_t>(vex_mmmmm_mapping, token)) {

                if (opt_mmmmm_bits)
                    return false;

                opt_mmmmm_bits = map_get_value<std::string_view>(vex_mmmmm_mapping, token);
                continue;
            }

            if (map_has_key<std::string_view, uint8_t>(vex_rex_w_mapping, token)) {

                if (opt_rex_w_bit)
                    return false;

                opt_rex_w_bit = map_get_value<std::string_view>(vex_rex_w_mapping, token);
                continue;
            }
        }

        if (!opt_mmmmm_bits)
            return false;

        result = vex_prefix_spec_t(opt_l_bit, opt_pp_bits, opt_mmmmm_bits.value(), opt_rex_w_bit);
        return true;
    }

    bool build_vex_prefix_vec(const cmdline_t& cmdline, const vex_prefix_spec_t& spec, std::vector<uint8_t>& result) {
        
        // Check preconditions.
        //

        const auto has_r_bit{ cmdline.opt_rex_rxb_bits && (cmdline.opt_rex_rxb_bits.value() & 0b100) };
        const auto has_x_bit{ cmdline.opt_rex_rxb_bits && (cmdline.opt_rex_rxb_bits.value() & 0b010) };
        const auto has_b_bit{ cmdline.opt_rex_rxb_bits && (cmdline.opt_rex_rxb_bits.value() & 0b001) };
        const auto has_explicit_mmmmm_bits{ spec.mmmmm_bits != 0b01 };
        const auto has_w_bit{ spec.opt_rex_w_bit && spec.opt_rex_w_bit.value() };

        if (cmdline.x86_mode == x86_mode_t::X86_32) {

            if (has_r_bit || has_x_bit)
                return false;
        }

        const auto requires_3b{ has_x_bit || has_b_bit || has_explicit_mmmmm_bits || has_w_bit };
        if (requires_3b && cmdline.opt_force_prefix_size && cmdline.opt_force_prefix_size.value() != vex_prefix_size_t::SIZE_3B)
            return false;

        vex_prefix_size_t final_size{};
        if (requires_3b)
            final_size = vex_prefix_size_t::SIZE_3B;
        else
            final_size = cmdline.opt_force_prefix_size ? cmdline.opt_force_prefix_size.value() : vex_prefix_size_t::SIZE_2B;

        // Build prefix bytes.
        //

        uint8_t r_bit{};
        uint8_t xb_bits{};
        uint8_t mmmmm_bits{};
        uint8_t w_bit{};
        uint8_t vvvv_bits{};
        uint8_t l_bit{};
        uint8_t pp_bits{};

        if (cmdline.x86_mode == x86_mode_t::X86_32) {

            r_bit = 0b01;
            xb_bits = 0b11;
        }
        else {

            const auto& opt_rxb_bits{ cmdline.opt_rex_rxb_bits };
            if (!opt_rxb_bits) {

                r_bit = 0b01;
                xb_bits = 0b11;
            }
            else {

                const auto rxb_bits{ opt_rxb_bits.value() };
                r_bit = ~(rxb_bits >> 2) & 0b01;
                xb_bits = ~(rxb_bits & 0b11) & 0b11;
            }
        }

        mmmmm_bits = spec.mmmmm_bits;
        w_bit = spec.opt_rex_w_bit ? spec.opt_rex_w_bit.value() : 0;
        vvvv_bits = cmdline.opt_vex_vvvv_bits ? ((~cmdline.opt_vex_vvvv_bits.value()) & 0b1111) : 0b1111;
        l_bit = spec.opt_l_bit ? spec.opt_l_bit.value() : 0;
        pp_bits = spec.opt_pp_bits ? spec.opt_pp_bits.value() : 0b00;

        std::vector<uint8_t> tmp_result;
        if (final_size == vex_prefix_size_t::SIZE_2B) {

            const uint8_t byte1{ static_cast<uint8_t>((r_bit << 7) + (vvvv_bits << 3) + (l_bit << 2) + pp_bits) };

            tmp_result.push_back(0xC5);
            tmp_result.push_back(byte1);
        }
        else {

            const uint8_t byte1{ static_cast<uint8_t>((r_bit << 7) + (xb_bits << 5) + mmmmm_bits) };
            const uint8_t byte2{ static_cast<uint8_t>((w_bit << 7) + (vvvv_bits << 3) + (l_bit << 2) + pp_bits) };

            tmp_result.push_back(0xC4);
            tmp_result.push_back(byte1);
            tmp_result.push_back(byte2);
        }

        result = tmp_result;
        return true;
    }
}