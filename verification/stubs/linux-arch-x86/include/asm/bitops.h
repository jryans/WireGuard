/*
 * Vigor: Replace inline asm with generic versions since KLEE can't handle
 * assembly.
 */

#include <asm-generic/bitops.h>