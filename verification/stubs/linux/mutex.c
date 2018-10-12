#include <linux/mutex.h>

void __mutex_init(struct mutex *lock, const char *name,
		  struct lock_class_key *key)
{
}

void mutex_lock(struct mutex *lock)
{
}

void mutex_unlock(struct mutex *lock)
{
}
