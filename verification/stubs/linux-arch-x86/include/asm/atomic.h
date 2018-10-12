/*
 * Vigor: Atomic functions replaced with non-atomic versions since we only
 * simulate a single core.  This also gets around inline assembly, which KLEE
 * can't handle.
 */

#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/kconfig.h>
#include <linux/types.h>
#include <asm-generic/cmpxchg.h>

#define ATOMIC_INIT(i) { (i) }

static inline int atomic_read(const atomic_t *v)
{
	return v->counter;
}

static inline void atomic_set(atomic_t *v, int i)
{
	v->counter = i;
}

static inline int atomic_cmpxchg(atomic_t *v, int oldv, int newv)
{
	int mem = atomic_read(v);
	if (mem == oldv) {
		atomic_set(v, newv);
	}
	return mem;
}

static inline void atomic_inc(atomic_t *v)
{
	v->counter++;
}

static inline int atomic_inc_return(atomic_t *v)
{
	atomic_inc(v);
	return v->counter;
}

static inline void atomic_dec(atomic_t *v)
{
	v->counter--;
}

static inline bool atomic_dec_and_test(atomic_t *v)
{
	atomic_dec(v);
	return v->counter == 0;
}

#define ATOMIC64_INIT(i) { (i) }

static inline long atomic64_read(const atomic64_t *v)
{
	return v->counter;
}

static inline void atomic64_set(atomic64_t *v, long i)
{
	v->counter = i;
}

static inline long atomic64_cmpxchg(atomic64_t *v, long oldv, long newv)
{
	long mem = atomic64_read(v);
	if (mem == oldv) {
		atomic64_set(v, newv);
	}
	return mem;
}

static inline void atomic64_inc(atomic64_t *v)
{
	v->counter++;
}

static inline long atomic64_inc_return(atomic64_t *v)
{
	atomic64_inc(v);
	return v->counter;
}

static inline void atomic64_dec(atomic64_t *v)
{
	v->counter--;
}

static inline bool atomic64_dec_and_test(atomic64_t *v)
{
	atomic64_dec(v);
	return v->counter == 0;
}

static inline long atomic64_dec_if_positive(atomic64_t *v)
{
	long value = atomic64_read(v);
	value--;
	if (value < 0) {
		atomic64_set(v, value);
	}
	return value;
}

#endif // _ASM_ATOMIC_H
