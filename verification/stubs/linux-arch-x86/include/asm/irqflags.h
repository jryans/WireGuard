/*
 * Vigor: Replace inline asm with generic versions since KLEE can't handle
 * assembly.
 */

#ifndef _ASM_IRQFLAGS_H
#define _ASM_IRQFLAGS_H

#define arch_local_save_flags arch_local_save_flags
static unsigned long arch_local_save_flags(void)
{
	return 0;
}

#define arch_local_irq_restore arch_local_irq_restore
static void arch_local_irq_restore(unsigned long flags)
{
}

#include <asm-generic/irqflags.h>

#endif // _ASM_IRQFLAGS_H