#ifndef EE_PE_UNPADDED_TYPES_IG
#define EE_PE_UNPADDED_TYPES_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"
#include "ee/detail/pe/constants.h"

#pragma pack(push, 1)

typedef struct {

    ee_uint16_t machine;
    ee_uint16_t number_of_sections;
    ee_uint32_t time_date_stamp;
    ee_uint32_t pointer_to_symbol_table;
    ee_uint32_t number_of_symbols;
    ee_uint16_t size_of_optional_header;
    ee_uint16_t characteristics;

} ee_prv_pe_coff_file_header_t;

typedef struct {

    ee_uint32_t virtual_address;
    ee_uint32_t size;

} ee_prv_pe_data_dir_t;

typedef struct {

    ee_uint16_t magic;
    ee_byte_t major_linker_version;
    ee_byte_t minor_linker_version;
    ee_uint32_t size_of_code;
    ee_uint32_t size_of_initialized_data;
    ee_uint32_t size_of_uninitialized_data;
    ee_uint32_t address_of_entry_point;
    ee_uint32_t base_of_code;
    ee_uint32_t base_of_data;
    ee_uint32_t image_base;
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
    ee_uint32_t size_of_stack_reserve;
    ee_uint32_t size_of_stack_commit;
    ee_uint32_t size_of_heap_reserve;
    ee_uint32_t size_of_heap_commit;
    ee_uint32_t loader_flags;
    ee_uint32_t number_of_rva_and_sizes;
    ee_prv_pe_data_dir_t data_dirs[EE_PRV_PE_COUNT_OPT_HEADER_DATA_DIRS];

} ee_prv_pe_optional_header_t;

typedef struct {

    ee_uint16_t magic;
    ee_byte_t major_linker_version;
    ee_byte_t minor_linker_version;
    ee_uint32_t size_of_code;
    ee_uint32_t size_of_initialized_data;
    ee_uint32_t size_of_uninitialized_data;
    ee_uint32_t address_of_entry_point;
    ee_uint32_t base_of_code;
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

} ee_prv_pe_optional_header_plus_t;

typedef struct {

    ee_ascii_char_t name[8];
    ee_uint32_t virtual_size;
    ee_uint32_t virtual_address;
    ee_uint32_t size_of_raw_data;
    ee_uint32_t pointer_to_raw_data;
    ee_uint32_t pointer_to_relocations;
    ee_uint32_t pointer_to_linenumbers;
    ee_uint16_t number_of_relocations;
    ee_uint16_t number_of_linenumbers;
    ee_uint32_t characteristics;

} ee_prv_pe_section_header_t;

typedef struct {

    ee_uint32_t import_lookup_table;
    ee_uint32_t time_date_stamp;
    ee_uint32_t forwarder_chain;
    ee_uint32_t name;
    ee_uint32_t import_address_table;

} ee_prv_pe_import_directory_entry_t;

typedef struct {

    ee_uint32_t characteristics;
    ee_uint32_t time_date_stamp;
    ee_uint16_t major_version;
    ee_uint16_t minor_version;
    ee_uint32_t name;
    ee_uint32_t base;
    ee_uint32_t number_of_functions;
    ee_uint32_t number_of_names;
    ee_uint32_t address_of_functions;
    ee_uint32_t address_of_names;
    ee_uint32_t address_of_name_ordinals;

} ee_prv_pe_export_directory_entry_t;

#pragma pack(pop)

#endif
