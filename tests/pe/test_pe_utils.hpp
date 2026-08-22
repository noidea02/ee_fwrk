#include <stdint.h>
#include <string.h>

#include <string_view>
#include <vector>

#include <ee/ee_pe.h>

#include <ee_support/utils.hpp>

using namespace ee_supp;

namespace ee_test {

    inline ee_bool_t read_data_from_byte_vec_ctx(void* ctx, ee_uint32_t offset, ee_size_t size, void* out) {

        if (!ctx || !out)
            return EE_FALSE;

        const auto vec{ static_cast<const std::vector<uint8_t>*>(ctx) };
        if (offset > vec->size() || size > (vec->size() - offset))
            return EE_FALSE;

        memcpy(out, &vec->at(offset), size);
        return EE_TRUE;
    }

    inline bool read_pe_test_data(std::string_view name, std::vector<uint8_t>& out) {

        const auto path{ std::filesystem::path(EE_PE_TEST_DATA_DIR) / name };
        return read_hex_file(path, out);
    }

    inline ee_pe_rii_callbacks create_pe_rii_callbacks_with_crt() {

        ee_pe_rii_callbacks out{};
        out.calloc = calloc;
        out.free = free;

        return out;
    }
}