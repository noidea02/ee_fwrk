# ee_fwrk

Header-only C library for assisting with x86 binary analysis. Provides functions for:

- x86 instruction decoding & formatting ([ee_x86](#ee_x86)).
- PE image file parsing ([ee_pe](#ee_pe)).

The library adheres to C89 language rules and does not have any external dependencies (CRT-less).

&nbsp;

## ee_x86
### Overview

ee_x86 implements the x86 instruction decoder and formatter. As of today, the following instruction set features and extensions are supported:

- 16, 32 and 64-bit mode
- General-Purpose instructions
- x87
- MMX
- SSE, SSE2, SSE3, SSSE3, SSE4, SSE4.1, SSE4.2
- AES-NI, SHA
- AVX, AVX2
- FMA
- BMI
- AMD-V, Intel VT-x

Other extensions such as AVX-512, AMX and APX are not supported (yet).

ee_x86 is thread-safe by default and increases the binary size by roughly 275 kb (decoder: 190 kb). Verification is done by using a proprietary structure-aware differential fuzzer,
which uses MSDis (Microsoft Disassembler) as reference. Since MSDis does not support 16-bit mode, only code paths related to 32 and 64-bit mode are subjected to fuzzing.

ee_x86 and fuzzer performance (16 threads):
```
[00007FD8] Consumed a total of 4.426.772.700 instructions (77% validity; 43.244.402.972 bytes) in 6 minutes
[00007FD8] x86 instruction generator check has finished, press ENTER to exit
```
The fuzzer is not part of this repository.

&nbsp;

### Quickstart

In order to integrate ee_x86 into a project, the "include" directory of this repository must be added to the project's include path. No additional steps are required except for including
the relevant header files in the code.

Minimal C++ sample:
```c++
#include <stdint.h>
#include <iostream>

// Define ee_fwrk symbols in this compilation unit.
#define EE_BASE_IMPL
#define EE_X86_IMPL
#define EE_X86_FORMAT_IMPL

#include <ee/ee_x86.h> // x86 decoder.
#include <ee/ee_x86_format.h> // x86 instruction formatter; can be omitted if textual representation is not needed.

int main(int argc, char* argv[]) {

    const uint8_t instruction[]{ 0xf0, 0x4f, 0x0f, 0xb1, 0xb4, 0xec, 0x44, 0x55, 0x66, 0x77 };
    ee_x86_disasm_output_t disasm_out{};

    // Decode instruction.
    if (!ee_x86_disasm(EE_X86_MODE_64, instruction, sizeof(instruction), &disasm_out)) {
        return 1; // Invalid instruction encoding.
    }

    // Inspect raw decoder output.
    std::cout << disasm_out.num_instruction_bytes << " bytes have been decoded" << std::endl;

    // Inspect output further if needed.
    if (disasm_out.instruction == EE_X86_INSTRUCTION_CMPXCHG && disasm_out.num_operands == 2) {
        // ...
    }

    // Format decoder output.
    ee_uint64_t instruction_address{ 0x7ff000000000 };
    ee_ascii_char_t instruction_str[256]{};
    ee_size_t instruction_str_size{ sizeof(instruction_str) - 1 };

    if (!ee_x86_format(EE_X86_MODE_64, instruction_address, &disasm_out, instruction_str, &instruction_str_size)) {
        return 1; // Something went wrong, the string buffer may be too small.
    }

    std::cout << instruction_str << std::endl;
    return 0;
}
```

&nbsp;

## ee_pe
### Overview

ee_pe implements the PE image file parser. As of today, the parser supports extracting the following information from an image file:

- Machine type
- Image base
- Imports
- Exports
- Range info for .rdata, .data, and .text

ee_pe is NOT subjected to fuzzing or extended testing. While the expression "use at your own risk" is true for the entire ee_fwrk library, it is especially relevant for ee_pe.

&nbsp;

### Quickstart

Refer to pe_info.c from the samples directory.

&nbsp;
