/*
 * Vigor: Pointer ring data structure (a limited-size FIFO queue typically used
 * across multiple cores) can be simplified since we only simulate a single
 * core.  This also gets around inline assembly, which KLEE can't handle.
 */

#ifndef _LINUX_PTR_RING_H
#define _LINUX_PTR_RING_H

#include <linux/types.h>

#include <linux/gfp.h>

#include <klee.h>

struct ptr_ring {
	void *item;
};

static inline int ptr_ring_init(struct ptr_ring *r, int size, gfp_t flags)
{
	klee_assert((flags & GFP_KERNEL) == GFP_KERNEL);
	r->item = NULL;
	return 0;
}

static inline void ptr_ring_cleanup(struct ptr_ring *r, void (*destroy)(void *))
{
	klee_assert(!destroy);
}

static inline bool __ptr_ring_empty(struct ptr_ring *r)
{
	return r->item == NULL;
}

static inline void *__ptr_ring_peek(struct ptr_ring *r)
{
	return r->item;
}

static inline void __ptr_ring_discard_one(struct ptr_ring *r)
{
	r->item = NULL;
}

static inline void *ptr_ring_consume_bh(struct ptr_ring *r)
{
	void *item = r->item;
	r->item = NULL;
	return item;
}

static inline int ptr_ring_produce_bh(struct ptr_ring *r, void *ptr)
{
	klee_assert(__ptr_ring_empty(r));
	r->item = ptr;
	return 0;
}

#endif // _LINUX_PTR_RING_H