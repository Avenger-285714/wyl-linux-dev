// SPDX-License-Identifier: MIT
/*
 * Copyright © 2020 Intel Corporation
 */

#ifdef CONFIG_X86
#include <asm/msr.h>
#endif

#include "i915_drv.h"
#include "librapl.h"

bool librapl_supported(const struct drm_i915_private *i915)
{
	/* Discrete cards require hwmon integration */
	if (IS_DGFX(i915))
		return false;

	return librapl_energy_uJ();
}

u64 librapl_energy_uJ(void)
{
#ifdef CONFIG_X86
	unsigned long long power;
	u32 units;

	if (rdmsrq_safe(MSR_RAPL_POWER_UNIT, &power))
		return 0;

	units = (power & 0x1f00) >> 8;

	if (rdmsrq_safe(MSR_PP1_ENERGY_STATUS, &power))
		return 0;

	return (1000000 * power) >> units; /* convert to uJ */
#else
	/* MSR access not available on non-x86 platforms */
	return 0;
#endif
}
