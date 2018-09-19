/* SPDX-License-Identifier: GPL-2.0 */
/*
 * include/linux/random.h
 *
 * Include file for the random number generator.
 */

#ifndef _LINUX_RANDOM_H
#define _LINUX_RANDOM_H

/* Vigor: Trimmed down to essential includes only. */

#include <linux/kernel.h>
#include <linux/once.h>

/* Vigor: Define just enough to compile. */

struct random_ready_callback {
	struct list_head list;
	void (*func)(struct random_ready_callback *rdy);
	struct module *owner;
};

#endif /* _LINUX_RANDOM_H */
