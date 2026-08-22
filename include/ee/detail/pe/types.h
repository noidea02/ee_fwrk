#ifndef EE_PE_TYPES_IG
#define EE_PE_TYPES_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_pe.h"
#include "ee/detail/pe/constants.h"
#include "ee/detail/pe/unpadded_types.h"

typedef enum {

    EE_PRV_PE_DATA_DIR_INDEX_EXPORT_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_IMPORT_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_RESOURCE_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_EXCEPTION_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_CERTIFICATE_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_BASE_RELOCATION_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_DEBUG,
    EE_PRV_PE_DATA_DIR_INDEX_ARCHITECTURE,
    EE_PRV_PE_DATA_DIR_INDEX_GLOBAL_PTR,
    EE_PRV_PE_DATA_DIR_INDEX_TLS_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_LOAD_CONFIG_TABLE,
    EE_PRV_PE_DATA_DIR_INDEX_BOUND_IMPORT,
    EE_PRV_PE_DATA_DIR_INDEX_IAT,
    EE_PRV_PE_DATA_DIR_INDEX_DELAY_IMPORT_DESCRIPTOR,
    EE_PRV_PE_DATA_DIR_INDEX_CLR_RUNTIME_HEADER,
    EE_PRV_PE_DATA_DIR_INDEX_RESERVED

} ee_prv_pe_data_dir_index_t;

typedef struct {

    ee_bool_t is_pe_plus_header;
    ee_byte_t major_linker_version;
    ee_byte_t minor_linker_version;
    ee_uint32_t size_of_code;
    ee_uint32_t size_of_initialized_data;
    ee_uint32_t size_of_uninitialized_data;
    ee_uint32_t address_of_entry_point;
    ee_uint32_t base_of_code;
    ee_uint32_t base_of_data;
    ee_uint64_t image_base;
    ee_uint32_t section_alignment;
    ee_uint32_t file_alignment;
    ee_uint16_t major_os_version;
    ee_uint16_t minor_os_version;
    ee_uint16_t major_image_version;
    ee_uint16_t minor_image_version;
    ee_uint16_t major_subsystem_version;
    ee_uint16_t minor_subsystem_version;
    ee_uint32_t win32_version_value;
    ee_uint32_t size_of_image;
    ee_uint32_t size_of_headers;
    ee_uint32_t check_sum;
    ee_uint16_t subsystem;
    ee_uint16_t dll_characteristics;
    ee_uint64_t size_of_stack_reserve;
    ee_uint64_t size_of_stack_commit;
    ee_uint64_t size_of_heap_reserve;
    ee_uint64_t size_of_heap_commit;
    ee_uint32_t loader_flags;
    ee_uint32_t number_of_rva_and_sizes;
    ee_prv_pe_data_dir_t data_dirs[EE_PRV_PE_COUNT_OPT_HEADER_DATA_DIRS];

} ee_prv_pe_unified_optional_header_t;

typedef struct {

    const ee_pe_rii_callbacks_t* callbacks;
    ee_uint32_t cur_fp;
    ee_prv_pe_coff_file_header_t coff_file_header;
    ee_prv_pe_unified_optional_header_t opt_header;
    ee_prv_pe_section_header_t* section_headers;
    ee_size_t num_section_headers;
    ee_pe_section_info_t rdata_section_info;
    ee_pe_section_info_t data_section_info;
    ee_pe_section_info_t idata_section_info;
    ee_pe_section_info_t iat_section_info;
    ee_pe_section_info_t edata_section_info;
    ee_pe_section_info_t text_section_info;
    ee_pe_import_info_t* imports;
    ee_size_t num_imports;
    ee_pe_symbol_info_t* exports;
    ee_size_t num_exports;

} ee_prv_pe_rii_state_t;

typedef struct {

    ee_uint32_t import_lookup_table_fp;
    ee_uint32_t import_lookup_table_rva;
    ee_uint32_t iat_fp;
    ee_uint32_t iat_rva;

} ee_prv_pe_imported_dll_info_t;

typedef struct {

    ee_size_t index;
    ee_uint64_t value;
    ee_size_t size;

} ee_prv_pe_import_lookup_table_entry_info_t;

typedef struct {

    ee_uint32_t ordinal_base;
    ee_uint32_t addr_table_fp;
    ee_size_t num_addr_table_entries;
    ee_uint32_t name_table_fp;
    ee_uint32_t ordinal_table_fp;

} ee_prv_pe_export_directory_info_t;

#endif
