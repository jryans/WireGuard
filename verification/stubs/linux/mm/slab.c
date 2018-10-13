#include <linux/slab.h>

#include <linux/gfp.h>

#include <string.h>
#include <klee.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void *__kmalloc(size_t size, gfp_t flags)
{
	// Ensure only supported flags are used.
	const gfp_t supported_flags = GFP_KERNEL | GFP_ATOMIC | __GFP_ZERO;
	klee_assert((flags | supported_flags) == supported_flags);
	void *ptr = malloc(size);
	if (!ptr) {
		return NULL;
	}
	if (flags & __GFP_ZERO) {
		memset(ptr, 0, size);
	}
	return ptr;
}

void kfree(const void *ptr)
{
	free((void *)ptr);
}

struct kmem_cache {
	unsigned int object_size;
};

struct kmem_cache *
kmem_cache_create(const char *name, size_t size, size_t align,
		  slab_flags_t flags, void (*ctor)(void *))
{
	klee_assert(flags == 0);
	klee_assert(!ctor);
	struct kmem_cache *cache = malloc(sizeof(struct kmem_cache));
	if (!cache) {
		return NULL;
	}
	cache->object_size = size;
	return cache;
}

void kmem_cache_destroy(struct kmem_cache *cache)
{
	free(cache);
}

void *kmem_cache_alloc(struct kmem_cache *cache, gfp_t flags)
{
	// Ensure only supported flags are used.
	const gfp_t supported_flags = GFP_KERNEL | GFP_ATOMIC;
	klee_assert((flags | supported_flags) == supported_flags);
	// TODO: Actually reuse objects as in the real cache?
	return malloc(cache->object_size);
}

void kmem_cache_free(struct kmem_cache *cache, void *obj)
{
	free(obj);
}
