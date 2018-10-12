/*
 * Check various kernel config options that are required to be set a certain
 * way for Vigor verification.
 */

#include <linux/kconfig.h>

#ifdef CONFIG_PARAVIRT
#error	"CONFIG_PARAVIRT uses global register variables, which Clang doesn't "
	"support. Please disable."
#endif

#ifdef CONFIG_PARAVIRT_CLOCK
#error 	"CONFIG_PARAVIRT_CLOCK uses global register variables, which Clang "
	"doesn't support. Please disable."
#endif

#ifdef CONFIG_PARAVIRT_SPINLOCKS
#error 	"CONFIG_PARAVIRT_SPINLOCKS uses global register variables, which Clang "
	"does not support. Please disable."
#endif

#ifdef CONFIG_SMP
#error 	"Vigor verification only supports one core. Please disable CONFIG_SMP."
#endif

#if CONFIG_NR_CPUS != 1
#error	"Vigor verification only supports one core. Please set CONFIG_NR_CPUS "
	"to 1."
#endif

#ifdef CONFIG_ARCH_HAS_REFCOUNT
#error	"CONFIG_ARCH_HAS_REFCOUNT implies inline asm, which KLEE does not "
	"support. Please disable."
#endif
