#include <stdint.h>

#include <filesystem>
#include <functional>
#include <optional>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include <ee/ee_pe.h>

#include <ee_support/scope_exit.hpp>
#include <ee_support/utils.hpp>

#include "test_pe_utils.hpp"

using namespace ee_supp;
using namespace ee_test;

namespace {

    struct symbol_info {

        std::optional<std::string> opt_name;
        std::optional<uint32_t> opt_ordinal;

        symbol_info(const std::string& name) : opt_name{ name } {}
        symbol_info(uint32_t ordinal) : opt_ordinal{ ordinal } {}
    };

    struct import_info {

        std::string name;
        std::vector<symbol_info> symbols;

        import_info(const std::string& name, const std::vector<symbol_info>& symbols) : name{ name }, symbols{ symbols } {}
    };

    struct image_info {

        ee_pe_machine_type_t machine_type;
        std::vector<import_info> imports;
        std::vector<symbol_info> exports;

        image_info(ee_pe_machine_type_t machine_type, const std::vector<import_info>& imports, const std::vector<symbol_info>& exports = {}) :
            machine_type{ machine_type }, imports{ imports }, exports{ exports } {}
    };

    std::optional<std::reference_wrapper<const ee_pe_import_info_t>> ee_pe_image_info_find_import(const ee_pe_image_info& image_info,
        const import_info& criteria) {

        const auto expected_module_name_low{ str_to_lower(criteria.name)};

        for (size_t i{}; i != image_info.num_imports; ++i) {

            const auto& cur_import{ image_info.imports[i] };
            if (!cur_import.module_name || str_to_lower(cur_import.module_name) != expected_module_name_low)
                continue;

            return std::optional(std::cref(cur_import));
        }

        return std::nullopt;
    }

    std::optional<std::reference_wrapper<const ee_pe_symbol_info_t>> ee_pe_import_info_find_symbol(const ee_pe_import_info_t& import_info,
        const symbol_info& criteria) {

        const auto opt_expected_symbol_name_low{ !criteria.opt_name ? std::nullopt : std::optional(str_to_lower(criteria.opt_name.value())) };

        for (size_t i{}; i != import_info.num_symbols; ++i) {

            const auto& cur_symbol{ import_info.symbols[i] };
            if (opt_expected_symbol_name_low) {

                if (!cur_symbol.name || str_to_lower(cur_symbol.name) != opt_expected_symbol_name_low)
                    continue;
            }
            else if (criteria.opt_ordinal) {

                if (cur_symbol.biased_ordinal == EE_PE_ORDINAL_UNUSED || cur_symbol.biased_ordinal != criteria.opt_ordinal.value())
                    continue;
            }
            else {

                std::terminate();
            }

            return std::optional(std::cref(cur_symbol));
        }

        return  std::nullopt;
    }

    void assert_ee_pe_image_info_expectations(const ee_pe_image_info& is, const image_info& expected) {

        ASSERT_EQ(is.machine, expected.machine_type);
        ASSERT_GE(is.num_imports, expected.imports.size());

        if (expected.imports.size()) {

            for (const auto& expected_import : expected.imports) {

                const auto opt_found_import{ ee_pe_image_info_find_import(is, expected_import) };
                ASSERT_TRUE(opt_found_import);

                const auto found_import{ opt_found_import.value().get() };
                ASSERT_GE(found_import.num_symbols, expected_import.symbols.size());

                for (const auto& expected_symbol : expected_import.symbols) {

                    const auto opt_found_symbol{ ee_pe_import_info_find_symbol(found_import, expected_symbol) };
                    ASSERT_TRUE(opt_found_symbol);
                }
            }
        }
    }
}

TEST(ee_fwrk_pe, read_image_info_with_nullptr_callbacks) {

    ee_pe_image_info* image_info{};
    ASSERT_EQ(ee_pe_read_image_info(nullptr, &image_info), EE_PE_FAILURE);
}

TEST(ee_fwrk_pe, read_image_info_with_nullptr_info) {

    ee_pe_rii_callbacks cbs{};
    cbs.calloc = calloc;
    cbs.free = free;
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = nullptr;

    ASSERT_EQ(ee_pe_read_image_info(&cbs, nullptr), EE_PE_FAILURE);
}

TEST(ee_fwrk_pe, read_image_info_with_nullptr_calloc) {

    ee_pe_rii_callbacks cbs{};
    cbs.calloc = nullptr;
    cbs.free = free;
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = nullptr;

    ee_pe_image_info* image_info{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &image_info), EE_PE_FAILURE);
}

TEST(ee_fwrk_pe, read_image_info_with_nullptr_free) {

    ee_pe_rii_callbacks cbs{};
    cbs.calloc = calloc;
    cbs.free = nullptr;
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = nullptr;

    ee_pe_image_info* image_info{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &image_info), EE_PE_FAILURE);
}

TEST(ee_fwrk_pe, read_image_info_with_nullptr_read_data) {

    ee_pe_rii_callbacks cbs{};
    cbs.calloc = calloc;
    cbs.free = free;
    cbs.read_data.cb = nullptr;
    cbs.read_data.ctx = nullptr;

    ee_pe_image_info* image_info{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &image_info), EE_PE_FAILURE);
}

TEST(ee_fwrk_pe, read_image_info_of_pe32_exe_standard) {

    std::vector<uint8_t> pe_content;
    ASSERT_TRUE(read_pe_test_data("pe32_exe_standard.hex", pe_content));

    auto cbs{ create_pe_rii_callbacks_with_crt() };
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = &pe_content;

    ee_pe_image_info* is{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &is), EE_PE_SUCCESS);
    ASSERT_NE(is, nullptr);

    const scope_exit se([is]() {
        ee_pe_free_image_info(is, free);
    });

    const std::vector<import_info> expected_imports{
        { "Kernel32.dll", { symbol_info("Sleep") } },
        { "User32.dll", { symbol_info("MessageBoxW") } },
        { "Ws2_32.dll", { symbol_info(3), symbol_info(4), symbol_info(23), symbol_info(111), symbol_info(115) } }
    };

    assert_ee_pe_image_info_expectations(*is, image_info(EE_PE_MACHINE_I386, expected_imports));
}

TEST(ee_fwrk_pe, read_image_info_of_pe64_exe_standard) {

    std::vector<uint8_t> pe_content;
    ASSERT_TRUE(read_pe_test_data("pe64_exe_standard.hex", pe_content));

    auto cbs{ create_pe_rii_callbacks_with_crt() };
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = &pe_content;

    ee_pe_image_info* is{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &is), EE_PE_SUCCESS);
    ASSERT_NE(is, nullptr);

    const scope_exit se([is]() {
        ee_pe_free_image_info(is, free);
    });

    const std::vector<import_info> expected_imports{
        { "Kernel32.dll", { symbol_info("Sleep") } },
        { "User32.dll", { symbol_info("MessageBoxW") } },
        { "Ws2_32.dll", { symbol_info(3), symbol_info(4), symbol_info(23), symbol_info(111), symbol_info(115) } }
    };

    assert_ee_pe_image_info_expectations(*is, image_info(EE_PE_MACHINE_AMD64, expected_imports));
}

TEST(ee_fwrk_pe, read_image_info_of_pe32_dll_standard) {

    std::vector<uint8_t> pe_content;
    ASSERT_TRUE(read_pe_test_data("pe32_dll_standard.hex", pe_content));

    auto cbs{ create_pe_rii_callbacks_with_crt() };
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = &pe_content;

    ee_pe_image_info* is{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &is), EE_PE_SUCCESS);
    ASSERT_NE(is, nullptr);

    const scope_exit se([is]() {
        ee_pe_free_image_info(is, free);
    });

    const std::vector<import_info> expected_imports{
        { "Kernel32.dll", { symbol_info("CloseHandle"), symbol_info("CreateIoCompletionPort") } },
        { "api-ms-win-crt-string-l1-1-0.dll", { symbol_info("strncpy") } }
    };

    const std::vector<symbol_info> expected_exports{
        symbol_info("DsCreateIoCompletionPort"), symbol_info("DsCloseHandle"), symbol_info("DsConcatStrings")
    };

    assert_ee_pe_image_info_expectations(*is, image_info(EE_PE_MACHINE_I386, expected_imports, expected_exports));
}

TEST(ee_fwrk_pe, read_image_info_of_pe64_dll_standard) {

    std::vector<uint8_t> pe_content;
    ASSERT_TRUE(read_pe_test_data("pe64_dll_standard.hex", pe_content));

    auto cbs{ create_pe_rii_callbacks_with_crt() };
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = &pe_content;

    ee_pe_image_info* is{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &is), EE_PE_SUCCESS);
    ASSERT_NE(is, nullptr);

    const scope_exit se([is]() {
        ee_pe_free_image_info(is, free);
    });

    const std::vector<import_info> expected_imports{
        { "Kernel32.dll", { symbol_info("CloseHandle"), symbol_info("CreateIoCompletionPort") } },
        { "api-ms-win-crt-string-l1-1-0.dll", { symbol_info("strncpy") } }
    };

    const std::vector<symbol_info> expected_exports{
        symbol_info("DsCreateIoCompletionPort"), symbol_info("DsCloseHandle"), symbol_info("DsConcatStrings")
    };

    assert_ee_pe_image_info_expectations(*is, image_info(EE_PE_MACHINE_AMD64, expected_imports, expected_exports));
}