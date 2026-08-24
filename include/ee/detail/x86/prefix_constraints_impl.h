#ifndef EE_X86_PREFIX_CONSTRAINTS_IMPL_IG
#define EE_X86_PREFIX_CONSTRAINTS_IMPL_IG

#ifndef EE_PRV_UNLOCK_DETAIL
#error "Direct inclusion of detail headers is forbidden."
#endif

#include "ee/detail/x86/constants.h"
#include "ee/detail/x86/prefix_constraints.h"

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NP = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NFX = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        EE_PRV_X86_CONSTRAINT_EMPTY
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_N66 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
        EE_PRV_X86_CONSTRAINT_EMPTY
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NF3 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        EE_PRV_X86_CONSTRAINT_EMPTY
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NF2_NF3 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        EE_PRV_X86_CONSTRAINT_EMPTY
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_66 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
    { EE_PRV_X86_CONSTRAINT_EMPTY }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_66_NFX = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
    {
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        EE_PRV_X86_CONSTRAINT_EMPTY
    }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_F2 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
    { EE_PRV_X86_CONSTRAINT_EMPTY }
};

static const ee_prv_x86_grp_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_F3 = {
    { EE_PRV_X86_PREFAM_GRP },
    EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
    { EE_PRV_X86_CONSTRAINT_EMPTY }
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_REX_W0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_W_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_REX_W1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_REX_W_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_REX_B1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_REX_B_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NP_REX_W0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F3
        }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_W_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NP_REX_W1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F3
        }
    },
    EE_PRV_X86_REX_W_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NP_REX_B0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F2,
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F3
        }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_B_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_N66_REX_W0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
            EE_PRV_X86_CONSTRAINT_EMPTY
        }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_W_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NF3_REX_B0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
            EE_PRV_X86_CONSTRAINT_EMPTY
        }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_B_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_NF3_REX_B1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_CONSTRAINT_EMPTY,
        {
            EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
            EE_PRV_X86_CONSTRAINT_EMPTY
        }
    },
    EE_PRV_X86_REX_B_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_66_REX_W0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_W_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_66_REX_W1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_MANDATORY_GRP_PREFIX_66,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_REX_W_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_F3_REX_W0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_W_MASK
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_F3_REX_W1 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_REX_W_MASK,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_grp_rex_prefix_constraints_t EE_PRV_X86_PREFCONS_GRP_F3_REX_B0 = {
    {
        { EE_PRV_X86_PREFAM_GRP_REX },
        EE_PRV_X86_MANDATORY_GRP_PREFIX_F3,
        { EE_PRV_X86_CONSTRAINT_EMPTY }
    },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_REX_B_MASK
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_NP_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_NP_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F38 = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F38_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F38_W0 = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    0,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F38_W0_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    0,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F38_W1 = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    1,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F3A = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F3A_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F3A_W0 = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_66_0F3A_W0_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_F2_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_F2,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_F2_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_F2,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_F3_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_F3,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_F3_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_PP_F3,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_NP_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_NP_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_NP_0F38 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W0xIG32 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W0xIG32_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W0xIG64 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W0xIG64_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W1 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    1,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F_W1_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F,
    1,
    1,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F38 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F38_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0xIG32 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0xIG32_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0xIG64 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W0xIG64_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W1 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    1,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_66_0F3A_W1_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    1,
    1,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_F2_0F3A_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_F2,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_F2_0F38 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_F2,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_F3_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_F3,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_F3_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_F3,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_128_F3_0F38 = {
    { EE_PRV_X86_PREFAM_VEX },
    0,
    EE_PRV_X86_VEX_PP_F3,
    EE_PRV_X86_VEX_MMMMM_0F38,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_NP_0F = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_NP_0F_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_NP,
    EE_PRV_X86_VEX_MMMMM_0F,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_CONSTRAINT_EMPTY,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F38_W0 = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    0,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F38_W0_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F38,
    0,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F3A_W0 = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F3A_W0_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    0,
    0,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F3A_W1 = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    1,
    1,
    EE_PRV_X86_CONSTRAINT_EMPTY
};

static const ee_prv_x86_vex_prefix_constraints_t EE_PRV_X86_PREFCONS_VEX_256_66_0F3A_W1_NVVVV = {
    { EE_PRV_X86_PREFAM_VEX },
    1,
    EE_PRV_X86_VEX_PP_66,
    EE_PRV_X86_VEX_MMMMM_0F3A,
    1,
    1,
    EE_PRV_X86_VEX_VVVV_UNUSED
};

static const ee_prv_x86_address_size_override_prefix_constraints_t EE_PRV_X86_PREFCONS_ASO_X86 = {
    { EE_PRV_X86_PREFAM_ADDRESS_SIZE_OVERRIDE },
    EE_TRUE
};

static const ee_prv_x86_address_size_override_prefix_constraints_t EE_PRV_X86_PREFCONS_ASO_X86N_X64 = {
    { EE_PRV_X86_PREFAM_ADDRESS_SIZE_OVERRIDE },
    EE_FALSE,
    EE_TRUE,
    EE_TRUE
};

#endif
