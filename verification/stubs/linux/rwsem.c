#include <linux/rwsem.h>

void __init_rwsem(struct rw_semaphore *sem, const char *name,
                  struct lock_class_key *key)
{
}