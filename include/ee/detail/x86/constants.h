#ifndef EE_X86_CONSTANTS_IG
#define EE_X86_CONSTANTS_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/ee_base.h"

#define EE_PRV_X86_GRP1_PREFIX_LOCK ((ee_byte_t)0xf0)
#define EE_PRV_X86_GRP1_PREFIX_REPNE ((ee_byte_t)0xf2)
#define EE_PRV_X86_GRP1_PREFIX_REP ((ee_byte_t)0xf3)
#define EE_PRV_X86_GRP1_PREFIX_XACQUIRE EE_PRV_X86_GRP1_PREFIX_REPNE
#define EE_PRV_X86_GRP1_PREFIX_XRELEASE EE_PRV_X86_GRP1_PREFIX_REP
#define EE_PRV_X86_GRP2_PREFIX_ES_OVR ((ee_byte_t)0x26)
#define EE_PRV_X86_GRP2_PREFIX_CS_OVR ((ee_byte_t)0x2e)
#define EE_PRV_X86_GRP2_PREFIX_SS_OVR ((ee_byte_t)0x36)
#define EE_PRV_X86_GRP2_PREFIX_DS_OVR ((ee_byte_t)0x3e)
#define EE_PRV_X86_GRP2_PREFIX_FS_OVR ((ee_byte_t)0x64)
#define EE_PRV_X86_GRP2_PREFIX_GS_OVR ((ee_byte_t)0x65)
#define EE_PRV_X86_GRP2_PREFIX_BRANCH_NOT_TAKEN EE_PRV_X86_GRP2_PREFIX_CS_OVR
#define EE_PRV_X86_GRP2_PREFIX_BRANCH_TAKEN EE_PRV_X86_GRP2_PREFIX_DS_OVR
#define EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR ((ee_byte_t)0x66)
#define EE_PRV_X86_GRP4_PREFIX_ADR_SIZE_OVR ((ee_byte_t)0x67)

#define EE_PRV_X86_REX_MIN ((ee_byte_t)0x40)
#define EE_PRV_X86_REX_MAX ((ee_byte_t)0x4f)
#define EE_PRV_X86_REX_W_MASK ((ee_byte_t)0x08)
#define EE_PRV_X86_REX_R_MASK ((ee_byte_t)0x04)
#define EE_PRV_X86_REX_X_MASK ((ee_byte_t)0x02)
#define EE_PRV_X86_REX_B_MASK ((ee_byte_t)0x01)

#define EE_PRV_X86_VEX3_INTRO ((ee_byte_t)0xc4)
#define EE_PRV_X86_VEX2_INTRO ((ee_byte_t)0xc5)

#define EE_PRV_X86_ESCAPE_OPCODE_0 ((ee_byte_t)0x0f)
#define EE_PRV_X86_ESCAPE_OPCODE_1_38 ((ee_byte_t)0x38)
#define EE_PRV_X86_ESCAPE_OPCODE_1_3A ((ee_byte_t)0x3a)

#define EE_PRV_X86_MANDATORY_GRP_PREFIX_66 EE_PRV_X86_GRP3_PREFIX_OPD_SIZE_OVR
#define EE_PRV_X86_MANDATORY_GRP_PREFIX_F2 EE_PRV_X86_GRP1_PREFIX_REPNE
#define EE_PRV_X86_MANDATORY_GRP_PREFIX_F3 EE_PRV_X86_GRP1_PREFIX_REP
#define EE_PRV_X86_MANDATORY_GRP_PREFIX_FX (EE_PRV_X86_MANDATORY_GRP_PREFIX_F2 | EE_PRV_X86_MANDATORY_GRP_PREFIX_F3)

#define EE_PRV_X86_ADDRESS_SIZE_OVERRIDE_PREFIX_67 ((ee_byte_t)0x67)

#define EE_PRV_X86_VEX_R_MASK ((ee_byte_t)0x80)
#define EE_PRV_X86_VEX_X_MASK ((ee_byte_t)0x40)
#define EE_PRV_X86_VEX_B_MASK ((ee_byte_t)0x20)
#define EE_PRV_X86_VEX_MMMMM_MASK ((ee_byte_t)0x1f)
#define EE_PRV_X86_VEX_W_MASK ((ee_byte_t)0x80)
#define EE_PRV_X86_VEX_VVVV_MASK ((ee_byte_t)0x78)
#define EE_PRV_X86_VEX_L_MASK ((ee_byte_t)0x04)
#define EE_PRV_X86_VEX_PP_MASK ((ee_byte_t)0x03)

#define EE_PRV_X86_VEX_MMMMM_0F ((ee_byte_t)0x01)
#define EE_PRV_X86_VEX_MMMMM_0F38 ((ee_byte_t)0x02)
#define EE_PRV_X86_VEX_MMMMM_0F3A ((ee_byte_t)0x03)
#define EE_PRV_X86_VEX_PP_NP ((ee_byte_t)0x00)
#define EE_PRV_X86_VEX_PP_66 ((ee_byte_t)0x01)
#define EE_PRV_X86_VEX_PP_F2 ((ee_byte_t)0x03)
#define EE_PRV_X86_VEX_PP_F3 ((ee_byte_t)0x02)
#define EE_PRV_X86_VEX_VVVV_UNUSED ((ee_byte_t)0x0f)

#define EE_PRV_X86_MODRM_MOD_MASK ((ee_byte_t)0xc0)
#define EE_PRV_X86_MODRM_RM_MASK ((ee_byte_t)0x07)
#define EE_PRV_X86_MODRM_REG_MASK ((ee_byte_t)0x38)

#define EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_BRANCH_HINT ((ee_uint32_t)0x01 << 0)
#define EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_PSEUDONYMIZATION ((ee_uint32_t)0x01 << 1)
#define EE_PRV_X86_OPCIDF_IGNORE_REX_W_IN_32_BIT_MODE ((ee_uint32_t)0x01 << 2)
#define EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_LOCK ((ee_uint32_t)0x01 << 3) /* With LOCK */
#define EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WIWO_LOCK ((ee_uint32_t)0x01 << 4) /* With or without LOCK */
#define EE_PRV_X86_OPCIDF_XACQUIRE_XRELEASE_ENABLED_WI_WIWO_LOCK ((ee_uint32_t)0x01 << 5) /* XACQUIRE with LOCK, XRELEASE with or without LOCK */
#define EE_PRV_X86_OPCIDF_ELIGIBLE_FOR_BND_PREFIX ((ee_uint32_t)0x01 << 6)

#define EE_PRV_X86_CONSTRAINT_EMPTY -1

#endif
