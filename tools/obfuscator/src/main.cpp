#include <iostream>
#include <string_view>
#include <vector>

#define EE_BASE_IMPL
#include <ee/ee_base.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace fs = std::filesystem;

namespace {

    int enter_obfuscate_mode(const std::vector<std::string>& cmdline);
    int enter_deobfuscate_mode(const std::vector<std::string>& cmdline);
}

int main(int argc, char* argv[]) {

    const std::vector<std::string> cmdline(argv + 1, argv + argc);
    if (cmdline.empty())
        return 1;

    if (cmdline.at(0) == "obfuscate")
        return enter_obfuscate_mode(cmdline);

    if (cmdline.at(0) == "deobfuscate")
        return enter_deobfuscate_mode(cmdline);

    return 1;
}

namespace {

    std::optional<std::vector<ee_byte_t>> hex_str_to_byte_vec(std::string_view hs) {

        std::vector<ee_byte_t> out;
        out.reserve(hs.size() / 3);

        int b{ -1 };
        for (const auto chr : hs) {

            if (chr == '\\') {

                if (b == -1)
                    continue;

                out.push_back(static_cast<ee_byte_t>(b));
                b = -1;

                continue;
            }

            if (chr == 'x')
                continue;

            if (b == -1)
                b = 0;

            const auto opt_nibble{ hex_char_to_byte(chr) };
            if (!opt_nibble)
                return std::nullopt;

            b = (b << 4) | opt_nibble.value();
        }

        if (b != -1)
            out.push_back(static_cast<ee_byte_t>(b));

        return out;
    }

    int enter_obfuscate_mode(const std::vector<std::string>& cmdline) {

        if (cmdline.size() < 2)
            return 1;

        const auto in{ cmdline.at(1) };
        const auto out_size{ in.size() };
        const auto out{ std::make_unique<ee_byte_t[]>(out_size) };

        ee_obfuscate_str(in.c_str(), out.get(), out_size);

        const std::span<const ee_byte_t> out_view(out.get(), out.get() + out_size);
        const auto hex_str{ byte_range_to_hex_str(out_view) };
        
        std::cout << hex_str << std::endl;
        return 0;
    }

    int enter_deobfuscate_mode(const std::vector<std::string>& cmdline) {

        if (cmdline.size() < 2)
            return 1;

        const auto in{ cmdline.at(1) };
        const auto opt_byte_vec{ hex_str_to_byte_vec(in) };

        if (!opt_byte_vec)
            return 1;

        const auto& byte_vec{ opt_byte_vec.value() };
        const auto out_size{ byte_vec.size() + 1 };
        const auto out{ std::make_unique<ee_char8_t[]>(out_size) };

        ee_deobfuscate_str(byte_vec.data(), byte_vec.size(), out.get(), out_size);

        std::cout << out.get() << std::endl;
        return 0;
    }
}