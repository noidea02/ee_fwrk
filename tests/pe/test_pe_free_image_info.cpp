#include <stdint.h>

#include <filesystem>
#include <vector>

#include <gtest/gtest.h>

#include <ee/ee_pe.h>

#include <ee_support/utils.hpp>

#include "test_pe_utils.hpp"

using namespace ee_supp;
using namespace ee_test;

TEST(ee_fwrk_pe, free_image_info_with_nullptr) {
    ee_pe_free_image_info(nullptr, free);
}

TEST(ee_fwrk_pe, free_image_info_with_valid_ptr) {

    std::vector<uint8_t> pe_content;
    ASSERT_TRUE(read_pe_test_data("pe64_exe_standard.hex", pe_content));

    auto cbs{ create_pe_rii_callbacks_with_crt() };
    cbs.read_data.cb = read_data_from_byte_vec_ctx;
    cbs.read_data.ctx = &pe_content;

    ee_pe_image_info* image_info{};
    ASSERT_EQ(ee_pe_read_image_info(&cbs, &image_info), EE_PE_SUCCESS);

    ee_pe_free_image_info(image_info, free);
}