#include <linux/percpu.h>

// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);
extern void free(void *__ptr);

void __percpu *__alloc_percpu_gfp(size_t size, size_t align, gfp_t gfp)
{
  return __alloc_percpu(size, align);
}

void __percpu *__alloc_percpu(size_t size, size_t align)
{
  return malloc(size);
}

void free_percpu(void __percpu *__pdata)
{
  free(__pdata);
}
