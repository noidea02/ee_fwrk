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

    ee_uint32_t fp;
    ee_uint32_t rva;
    ee_uint32_t raw_size;
    ee_uint32_t loaded_size;

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

struct ee_pe_image_info  {

    ee_int32_t reserved;
    ee_pe_machine_type_t machine;
    ee_uint64_t image_base;
    ee_pe_import_info_t* imports;
    ee_size_t num_imports;
    ee_pe_symbol_info_t* exports;
    ee_size_t num_exports;
    ee_pe_section_info_t rdata_section_info;
    ee_pe_section_info_t data_section_info;
    ee_pe_section_info_t text_section_info;
};

/* Constants
*/

#define EE_PE_ORDINAL_UNUSED -1

/* Implementation
*/

#ifdef EE_PE_IMPL
    #define EE_PRV_UNLOCK_DETAIL
    #include "ee/detail/pe/parser_impl.h"
    #undef EE_PRV_UNLOCK_DETAIL
#endif

#endif
