#pragma once

#include <algorithm>
#include <array>
#include <exception>
#include <memory>
#include <ranges>

#include <ee/ee_x86.h>

static_assert(EE_GET_ARRAY_LEN(ee_x86_disasm_output::operands) >= 3);

bool operator==(const ee_x86_operand_t& in0, const ee_x86_operand_t& in1);

namespace ee_test {

    struct x86_prefix_state {

        ee_bool_t operand_size_override;
        ee_bool_t address_size_override;
    };

    struct x86_ptr_opd {

        ee_x86_pointer_type_t ptr_type;
        ee_x86_gpp_register_type_t base_reg;
        ee_x86_gpp_register_type_t index_reg;
    };

    struct x86_ptr_opd_with_extended_base {

        ee_x86_pointer_type_t ptr_type;
        ee_x86_numbered_register_t numbered_register;
    };

    struct x86_imm_opd {

        ee_uint8_t num_bits;
        ee_int64_t value;
    };

    struct x86_rel_opd {

        ee_uint8_t num_bits;
        ee_int32_t value;
    };

    struct x86_farptr_opd {

        ee_uint16_t selector;
        ee_uint8_t offset_num_bits;
        ee_uint32_t offset;
    };

    struct x86_numbered_reg_opd {

        ee_x86_register_type_t type;
        ee_uint32_t size_bits;
        ee_uint32_t index;
    };

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_two_operands(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_ptr_opd& opd0, ee_x86_gpp_register_type_t opd1) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 2;
        
        tmp_out.operands[0].type = EE_X86_OPERAND_POINTER;
        tmp_out.operands[0].un.pointer = ee_x86_pointer_operand_t{ .type = opd0.ptr_type };
        tmp_out.operands[0].un.pointer.base_register.is_set = EE_TRUE;
        tmp_out.operands[0].un.pointer.base_register.un.reg1ster = opd0.base_reg;

        if (opd0.index_reg != EE_X86_GPP_REGISTER_NOT_EXISTING) {
            
            tmp_out.operands[0].un.pointer.index_register.is_set = EE_TRUE;
            tmp_out.operands[0].un.pointer.index_register.is_gpp = EE_TRUE;
            tmp_out.operands[0].un.pointer.index_register.un.general_purpose_register = opd0.index_reg;
        }

        tmp_out.operands[1].type = EE_X86_OPERAND_REGISTER;
        tmp_out.operands[1].un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE;
        tmp_out.operands[1].un.reg1ster.un.general_purpose_register = opd1;

        return tmp_out;
    }
    
    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_two_operands(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, ee_x86_gpp_register_type_t opd0, const x86_ptr_opd& opd1) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 2;

        tmp_out.operands[0].type = EE_X86_OPERAND_REGISTER;
        tmp_out.operands[0].un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE;
        tmp_out.operands[0].un.reg1ster.un.general_purpose_register = opd0;

        tmp_out.operands[1].type = EE_X86_OPERAND_POINTER;
        tmp_out.operands[1].un.pointer = ee_x86_pointer_operand_t{ .type = opd1.ptr_type };
        tmp_out.operands[1].un.pointer.base_register.is_set = EE_TRUE;
        tmp_out.operands[1].un.pointer.base_register.un.reg1ster = opd1.base_reg;

        if (opd1.index_reg != EE_X86_GPP_REGISTER_NOT_EXISTING) {

            tmp_out.operands[1].un.pointer.index_register.is_set = EE_TRUE;
            tmp_out.operands[1].un.pointer.index_register.is_gpp = EE_TRUE;
            tmp_out.operands[1].un.pointer.index_register.un.general_purpose_register = opd1.index_reg;
        }

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_one_operand(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_imm_opd& opd) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 1;

        tmp_out.operands[0].type = EE_X86_OPERAND_IMMEDIATE_VALUE;
        tmp_out.operands[0].un.immediate_value.value = opd.value;
        tmp_out.operands[0].un.immediate_value.value_num_bits = opd.num_bits;

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_one_operand(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_rel_opd& opd) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 1;

        tmp_out.operands[0].type = EE_X86_OPERAND_RELATIVE_ADDRESS;
        tmp_out.operands[0].un.relative_address.offset = opd.value;
        tmp_out.operands[0].un.relative_address.offset_num_bits = opd.num_bits;

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_one_operand(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_farptr_opd& opd) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 1;

        tmp_out.operands[0].type = EE_X86_OPERAND_FAR_POINTER;
        tmp_out.operands[0].un.far_pointer.cs_selector = opd.selector;
        tmp_out.operands[0].un.far_pointer.cs_offset_num_bits = opd.offset_num_bits;
        tmp_out.operands[0].un.far_pointer.cs_offset = opd.offset;

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_one_operand(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, ee_x86_gpp_register_type_t opd) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 1;

        tmp_out.operands[0].type = EE_X86_OPERAND_REGISTER;
        tmp_out.operands[0].un.reg1ster.type = EE_X86_REGISTER_GENERAL_PURPOSE;
        tmp_out.operands[0].un.reg1ster.un.general_purpose_register = opd;

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_three_operands(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_numbered_reg_opd& opd0, const x86_numbered_reg_opd& opd1, const x86_numbered_reg_opd& opd2) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 3;

        const std::array<x86_numbered_reg_opd, 3> opds{ opd0, opd1, opd2 };
        size_t opd_index{};

        for (const auto& opd : opds) {

            auto& opd_out{ tmp_out.operands[opd_index++] };
            opd_out.type = EE_X86_OPERAND_REGISTER;
            opd_out.un.reg1ster.type = opd.type;
            opd_out.un.reg1ster.un.numbered_register.index = opd.index;
            opd_out.un.reg1ster.un.numbered_register.size_bits = opd.size_bits;
        }

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_three_operands(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_numbered_reg_opd& opd0, const x86_numbered_reg_opd& opd1, const x86_ptr_opd& opd2) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 3;

        const std::array<x86_numbered_reg_opd, 2> reg_opds{ opd0, opd1};
        size_t opd_index{};

        for (const auto& opd : reg_opds) {

            auto& opd_out{ tmp_out.operands[opd_index++] };
            opd_out.type = EE_X86_OPERAND_REGISTER;
            opd_out.un.reg1ster.type = opd.type;
            opd_out.un.reg1ster.un.numbered_register.index = opd.index;
            opd_out.un.reg1ster.un.numbered_register.size_bits = opd.size_bits;
        }

        tmp_out.operands[2].type = EE_X86_OPERAND_POINTER;
        tmp_out.operands[2].un.pointer = ee_x86_pointer_operand_t{ .type = opd2.ptr_type };
        tmp_out.operands[2].un.pointer.base_register.is_set = EE_TRUE;
        tmp_out.operands[2].un.pointer.base_register.un.reg1ster = opd2.base_reg;

        if (opd2.index_reg != EE_X86_GPP_REGISTER_NOT_EXISTING) {

            tmp_out.operands[2].un.pointer.index_register.is_set = EE_TRUE;
            tmp_out.operands[2].un.pointer.index_register.is_gpp = EE_TRUE;
            tmp_out.operands[2].un.pointer.index_register.un.general_purpose_register = opd2.index_reg;
        }

        return tmp_out;
    }

    inline constexpr ee_x86_disasm_output create_x86_disasm_output_with_three_operands(ee_size_t inst_len, const x86_prefix_state& prefixes, ee_x86_instruction_type_t inst, const x86_numbered_reg_opd& opd0, const x86_numbered_reg_opd& opd1, const x86_ptr_opd_with_extended_base& opd2) {

        ee_x86_disasm_output tmp_out{};
        tmp_out.num_instruction_bytes = inst_len;
        tmp_out.prefixes.operand_size_override = prefixes.operand_size_override;
        tmp_out.prefixes.address_size_override = prefixes.address_size_override;
        tmp_out.instruction = inst;
        tmp_out.num_operands = 3;

        const std::array<x86_numbered_reg_opd, 2> reg_opds{ opd0, opd1 };
        size_t opd_index{};

        for (const auto& opd : reg_opds) {

            auto& opd_out{ tmp_out.operands[opd_index++] };
            opd_out.type = EE_X86_OPERAND_REGISTER;
            opd_out.un.reg1ster.type = opd.type;
            opd_out.un.reg1ster.un.numbered_register.index = opd.index;
            opd_out.un.reg1ster.un.numbered_register.size_bits = opd.size_bits;
        }

        tmp_out.operands[2].type = EE_X86_OPERAND_POINTER;
        tmp_out.operands[2].un.pointer = ee_x86_pointer_operand_t{ .type = opd2.ptr_type };
        tmp_out.operands[2].un.pointer.base_register.is_set = EE_TRUE;
        tmp_out.operands[2].un.pointer.base_register.is_extended = EE_TRUE;
        tmp_out.operands[2].un.pointer.base_register.un.ex_register = opd2.numbered_register;

        return tmp_out;
    }

    inline bool check_x86_disasm_output_prefixes_equality(const ee_x86_disasm_output& in0, const ee_x86_disasm_output& in1) {
        
        if (sizeof(in0.prefixes) != sizeof(in1.prefixes))
            return false;
        
        return !memcmp(&in0.prefixes, &in1.prefixes, sizeof(in0.prefixes));
    }

    inline bool check_x86_disasm_output_operands_equality(const ee_x86_disasm_output& in0, const ee_x86_disasm_output& in1) {

        if (in0.num_operands != in1.num_operands)
            return false;

        return std::ranges::equal(in0.operands, in1.operands);
    }
}

inline bool operator==(const ee_x86_disasm_output& in0, const ee_x86_disasm_output& in1) {

    using namespace ee_test;

    return in0.instruction == in1.instruction
        && in0.num_instruction_bytes == in1.num_instruction_bytes
        && check_x86_disasm_output_prefixes_equality(in0, in1)
        && check_x86_disasm_output_operands_equality(in0, in1);
}

inline bool operator==(const ee_x86_far_pointer_operand_t& in0, const ee_x86_far_pointer_operand_t& in1) {

    return in0.cs_offset == in1.cs_offset
        && in0.cs_offset_num_bits == in1.cs_offset_num_bits
        && in0.cs_selector == in1.cs_selector;
}

inline bool operator==(const ee_x86_immediate_value_operand_t& in0, const ee_x86_immediate_value_operand_t& in1) {

    return in0.value == in1.value
        && in0.value_num_bits == in1.value_num_bits;
}

inline bool operator==(const ee_x86_numbered_register_t& in0, const ee_x86_numbered_register_t& in1) {

    return in0.index == in1.index
        && in0.size_bits == in1.size_bits;
}

inline bool operator==(const ee_x86_pointer_operand_base_register_t& in0, const ee_x86_pointer_operand_base_register_t& in1) {

    if (in0.is_set != in1.is_set || in0.is_extended != in1.is_extended)
        return false;

    if (in0.is_extended)
        return in0.un.ex_register == in1.un.ex_register;

    return in0.un.reg1ster == in1.un.reg1ster;
}

inline bool operator==(const ee_x86_pointer_operand_index_register_t& in0, const ee_x86_pointer_operand_index_register_t& in1) {

    if (in0.is_set != in1.is_set || in0.is_gpp != in1.is_gpp || in0.is_gpp_ex != in1.is_gpp_ex || in0.is_vec != in1.is_vec)
        return false;

    if (in0.is_gpp_ex || in0.is_vec)
        return in0.un.numbered_register == in1.un.numbered_register;

    return in0.un.general_purpose_register == in1.un.general_purpose_register;
}

inline bool operator==(const ee_x86_pointer_operand_t& in0, const ee_x86_pointer_operand_t& in1) {

    return in0.base_register == in1.base_register
        && in0.displacement == in1.displacement
        && in0.displacement_num_bits == in1.displacement_num_bits
        && in0.index_register == in1.index_register
        && in0.scaling == in1.scaling
        && in0.segment_register == in1.segment_register
        && in0.type == in1.type
        && in0.uses_rip_relative_addressing == in1.uses_rip_relative_addressing;
}

inline bool operator==(const ee_x86_register_operand_t& in0, const ee_x86_register_operand_t& in1) {

    if (in0.type != in1.type)
        return false;

    switch (in0.type) {

    case EE_X86_REGISTER_GENERAL_PURPOSE:

        return in0.un.general_purpose_register == in1.un.general_purpose_register;

    case EE_X86_REGISTER_SEGMENT:

        return in0.un.segment_register == in1.un.segment_register;
    }

    return in0.un.numbered_register == in1.un.numbered_register;
}

inline bool operator==(const ee_x86_relative_address_operand_t& in0, const ee_x86_relative_address_operand_t& in1) {

    return in0.offset == in1.offset
        && in0.offset_num_bits == in1.offset_num_bits;
}

inline bool operator==(const ee_x86_operand_t& in0, const ee_x86_operand_t& in1) {

    if (in0.type != in1.type)
        return false;

    if (in0.type == EE_X86_OPERAND_NOT_EXISTING)
        return true;

    switch (in0.type) {

    case EE_X86_OPERAND_FAR_POINTER: 
        
        return in0.un.far_pointer == in1.un.far_pointer;

    case EE_X86_OPERAND_IMMEDIATE_VALUE:

        return in0.un.immediate_value == in1.un.immediate_value;

    case EE_X86_OPERAND_POINTER:

        return in0.un.pointer == in1.un.pointer;

    case EE_X86_OPERAND_REGISTER:

        return in0.un.reg1ster == in1.un.reg1ster;

    case EE_X86_OPERAND_RELATIVE_ADDRESS:

        return in0.un.relative_address == in1.un.relative_address;
    }

    std::terminate();
}