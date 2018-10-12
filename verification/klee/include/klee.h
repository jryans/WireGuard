/*
 * A small wrapper around KLEE intrinsics so they can be left in non-KLEE builds
 * with no effect. In addition, we copy definitions from the normal KLEE headers
 * instead of including them to avoid type conflicts between kernel headers and
 * the stdlib headers that klee.h includes.
 */

#ifndef _KLEE_H
#define _KLEE_H

#ifdef KLEE_VERIFICATION

void klee_make_symbolic(void *addr, size_t nbytes, const char *name);
int klee_range(int begin, int end, const char *name);
#define klee_assert(expr)                                                      \
	((expr) ? (void)(0) :                                                  \
		  __assert_fail(#expr, __FILE__, __LINE__,                     \
				__PRETTY_FUNCTION__))

#else // KLEE_VERIFICATION

static void klee_make_symbolic(void *addr, size_t nbytes, const char *name)
{
}
static int klee_range(int begin, int end, const char *name)
{
	return begin;
}
#define klee_assert(expr)

#endif // KLEE_VERIFICATION

#endif // _KLEE_H