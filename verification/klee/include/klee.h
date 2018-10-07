/*
 * A small wrapper around KLEE intrinsics so they can be left in non-KLEE builds
 * with no effect.
 */

// Define this to avoid redefintion errors via `inttypes.h` in `klee/klee.h`
#define _INTTYPES_H
#include <klee/klee.h>

#ifndef KLEE_VERIFICATION
void klee_make_symbolic(void *addr, size_t nbytes, const char *name) {}
int klee_range(int begin, int end, const char *name) {
  return begin;
}
#define klee_assert(expr)
#endif // !KLEE_VERIFICATION
