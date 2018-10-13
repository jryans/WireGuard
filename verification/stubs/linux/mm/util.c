#include <linux/mm.h>

#include <linux/gfp.h>
#include <linux/numa.h>
#include <linux/types.h>

#include <string.h>
#include <klee.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void *kvmalloc_node(size_t size, gfp_t flags, int node)
{
	// Ensure only supported flags are used.
	const gfp_t supported_flags = GFP_KERNEL | __GFP_ZERO;
	klee_assert((flags | supported_flags) == supported_flags);
	// GFP_KERNEL must be passed.
	klee_assert((flags & GFP_KERNEL) == GFP_KERNEL);
	// Specific nodes are not supported.
	klee_assert(node == NUMA_NO_NODE);
	void *ptr = malloc(size);
	if (!ptr) {
		return NULL;
	}
	if (flags & __GFP_ZERO) {
		memset(ptr, 0, size);
	}
	return ptr;
}

void kvfree(const void *addr)
{
	free((void *)addr);
}