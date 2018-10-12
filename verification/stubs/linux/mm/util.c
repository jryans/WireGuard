#include <linux/mm.h>

#include <linux/gfp.h>
#include <linux/numa.h>
#include <linux/types.h>

#include <klee.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void *kvmalloc_node(size_t size, gfp_t flags, int node)
{
	klee_assert((flags & GFP_KERNEL) == GFP_KERNEL);
	klee_assert(node == NUMA_NO_NODE);
	return malloc(size);
}

void kvfree(const void *addr)
{
	free((void *)addr);
}