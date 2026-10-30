#ifndef EE_PE_IG
#define EE_PE_IG

#include "ee/ee_base.h"

/* Functions
*/

typedef enum {

    EE_PE_SUCCESS,
    EE_PE_FAILURE,
    EE_PE_MALFORMED_FILE,
    EE_PE_PREDEFINED_LIMIT_REACHED,
    EE_PE_BAD_ALLOC

} ee_pe_error_t;

typedef struct ee_pe_rii_callbacks ee_pe_rii_callbacks_t;
typedef struct ee_pe_image_info ee_pe_image_info_t;

ee_pe_error_t ee_pe_read_image_info(const ee_pe_rii_callbacks_t* callbacks, ee_pe_image_info_t** info);

void ee_pe_free_image_info(ee_pe_image_info_t* info, ee_free_t free);

/* Types
*/

typedef enum {

    EE_PE_MACHINE_UNKNOWN,
    EE_PE_MACHINE_I386,
    EE_PE_MACHINE_AMD64

} ee_pe_machine_type_t;

struct ee_pe_rii_callbacks {

    ee_calloc_t calloc;
    ee_free_t free;

    ee_read_data_wrapper_t read_data;
};

typedef struct {

    ee_ascii_char_t* name;
    ee_uint32_t fp;
    ee_uint32_t rva;
    ee_uint32_t raw_size;
    ee_uint32_t loaded_size;
    ee_uint32_t flags;

} ee_pe_section_info_t;

typedef struct {

    ee_int32_t biased_ordinal; /* If EE_PE_ORDINAL_UNUSED, import / export by name; only unbiased ordinals can be used as table indices. */
    ee_ascii_char_t* name; /* If 0, import / export by ordinal. */
    ee_uint32_t fp; /* File pointer to IAT entry in case of import, pointer to code otherwise. */
    ee_uint32_t rva; /* RVA to IAT entry in case of import, RVA to code otherwise. */
    ee_ascii_char_t* forwarder_info; /* If not 0, this symbol acts as a forwarder for the symbol of a different DLL; in this case the ordinal, fp and rva fields are unset. */

} ee_pe_symbol_info_t;

typedef struct {

    ee_ascii_char_t* module_name;
    ee_pe_symbol_info_t* symbols;
    ee_size_t num_symbols;

} ee_pe_import_info_t;

typedef struct {

    ee_uint32_t fp;
    ee_uint32_t rva;

} ee_pe_entry_point_info_t;

struct ee_pe_image_info  {

    ee_int32_t reserved;
    ee_pe_machine_type_t machine;
    ee_uint64_t image_base;
    ee_pe_entry_point_info_t entry_point_info;
    ee_pe_import_info_t* imports;
    ee_size_t num_imports;
    ee_pe_symbol_info_t* exports;
    ee_size_t num_exports;
    ee_pe_section_info_t* sections;
    ee_size_t num_sections;
};

/* Constants
*/

#define EE_PE_ORDINAL_UNUSED -1

#define EE_PE_SECTION_FLAG_CNT_CODE 0x00000020
#define EE_PE_SECTION_FLAG_CNT_INITIALIZED_DATA 0x00000040
#define EE_PE_SECTION_FLAG_CNT_UNINITIALIZED_DATA 0x00000080
#define EE_PE_SECTION_FLAG_GPREL 0x00008000
#define EE_PE_SECTION_FLAG_LNK_NRELOC_OVFL 0x01000000
#define EE_PE_SECTION_FLAG_MEM_DISCARDABLE 0x02000000
#define EE_PE_SECTION_FLAG_MEM_NOT_CACHED 0x04000000
#define EE_PE_SECTION_FLAG_MEM_NOT_PAGED 0x08000000
#define EE_PE_SECTION_FLAG_MEM_SHARED 0x10000000
#define EE_PE_SECTION_FLAG_MEM_EXECUTE 0x20000000
#define EE_PE_SECTION_FLAG_MEM_READ 0x40000000
#define EE_PE_SECTION_FLAG_MEM_WRITE 0x80000000

/* Implementation
*/

#ifdef EE_PE_IMPL
    #define EE_PRV_UNLOCK_DETAIL
    #include "ee/detail/pe/parser_impl.h"
    #undef EE_PRV_UNLOCK_DETAIL
#endif

#endif
