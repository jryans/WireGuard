#include <linux/compiler_types.h>

#include <string.h>

noinline unsigned long __crypto_memneq(const void *a, const void *b,
				       size_t size)
{
        // Constant time implementation is not a requirement for verification.
        return memcmp(a, b, size);
}
