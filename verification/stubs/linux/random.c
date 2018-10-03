/*
 * Random number generation.
 * TODO: Should this actually be random...?
 */

#include <linux/random.h>

#include <linux/errno.h>

// #include <stdlib.h> for rand
extern int rand();

int add_random_ready_callback(struct random_ready_callback *rdy)
{
  return -EALREADY;
}

u32 get_random_u32(void)
{
  return rand();
}

void get_random_bytes(void *buf, int nbytes)
{
  int i;
  char *output = (char *)buf;
  for (i = 0; i < nbytes; i++) {
    output[i] = rand();
  }
}

int wait_for_random_bytes(void)
{
  return 0;
}