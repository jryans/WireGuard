#include <linux/slab.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void *__kmalloc(size_t size, gfp_t flags)
{
  return malloc(size);
}

void kfree(const void *ptr)
{
  free((void *)ptr);
}