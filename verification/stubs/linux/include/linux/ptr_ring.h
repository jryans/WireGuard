/*
 * Vigor: Pointer ring data structure (a limited-size FIFO queue typically used
 * across multiple cores) can be simplified since we only simulate a single
 * core.  This also gets around inline assembly, which KLEE can't handle.
 */

#ifndef _LINUX_PTR_RING_H
#define _LINUX_PTR_RING_H

#include <linux/types.h>

struct ptr_ring {
};

#endif // _LINUX_PTR_RING_H