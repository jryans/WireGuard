#include <linux/percpu.h>

#include <linux/gfp.h>

#include <klee.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void __percpu *__alloc_percpu_gfp(size_t size, size_t align, gfp_t flags)
{
	// Ensure only supported flags are used.
	const gfp_t supported_flags = GFP_KERNEL;
	klee_assert((flags | supported_flags) == supported_flags);
	// GFP_KERNEL must be passed.
	klee_assert((flags & GFP_KERNEL) == GFP_KERNEL);
	return __alloc_percpu(size, align);
}

void __percpu *__alloc_percpu(size_t size, size_t align)
{
	return malloc(size);
}

void free_percpu(void __percpu *__pdata)
{
	free(__pdata);
}
