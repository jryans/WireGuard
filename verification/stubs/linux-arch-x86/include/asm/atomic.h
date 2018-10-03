/*
 * Vigor: Atomic functions replaced with non-atomic versions since we only
 * simulate a single core.  This also gets around inline assembly, which KLEE
 * can't handle.
 */

#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/types.h>
#include <asm-generic/cmpxchg.h>

#define ATOMIC_INIT(i) { (i) }

static __always_inline int atomic_read(const atomic_t *v)
{
  return v->counter;
}

static __always_inline void atomic_set(atomic_t *v, int i)
{
  v->counter = i;
}

static __always_inline int atomic_cmpxchg(atomic_t *v, int oldv, int newv)
{
  int mem = v->counter;
  if (mem == oldv) {
    v->counter = newv;
  }
  return mem;
}

#define ATOMIC64_INIT(i) { (i) }

#endif // _ASM_ATOMIC_H