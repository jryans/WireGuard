/*
 * Vigor: Replace preempt functions with single thread versions since we only
 * simulate a single core.  This also gets around inline assembly, which KLEE
 * can't handle.
 */

/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_PREEMPT_H
#define _ASM_PREEMPT_H

#include <linux/stddef.h>

extern int _preempt_count;

static __always_inline int preempt_count(void)
{
	return _preempt_count;
}

static __always_inline volatile int *preempt_count_ptr(void)
{
	return &_preempt_count;
}

static __always_inline void preempt_count_set(int pc)
{
	*preempt_count_ptr() = pc;
}

/*
 * must be macros to avoid header recursion hell
 */
#define init_task_preempt_count(p)                                             \
	do {                                                                   \
		task_thread_info(p)->preempt_count = FORK_PREEMPT_COUNT;       \
	} while (0)

#define init_idle_preempt_count(p, cpu)                                        \
	do {                                                                   \
		task_thread_info(p)->preempt_count = PREEMPT_ENABLED;          \
	} while (0)

static __always_inline void set_preempt_need_resched(void)
{
}

static __always_inline void clear_preempt_need_resched(void)
{
}

static __always_inline bool test_preempt_need_resched(void)
{
	return false;
}

/*
 * The various preempt_count add/sub methods
 */

static __always_inline void __preempt_count_add(int val)
{
	*preempt_count_ptr() += val;
}

static __always_inline void __preempt_count_sub(int val)
{
	*preempt_count_ptr() -= val;
}

static __always_inline bool __preempt_count_dec_and_test(void)
{
	/*
	 * Because of load-store architectures cannot do per-cpu atomic
	 * operations; we cannot use PREEMPT_NEED_RESCHED because it might get
	 * lost.
	 */
	return !--*preempt_count_ptr() && tif_need_resched();
}

/*
 * Returns true when we need to resched and can (barring IRQ state).
 */
static __always_inline bool should_resched(int preempt_offset)
{
	return unlikely(preempt_count() == preempt_offset &&
			tif_need_resched());
}

#ifdef CONFIG_PREEMPT
extern asmlinkage void preempt_schedule(void);
#define __preempt_schedule() preempt_schedule()
extern asmlinkage void preempt_schedule_notrace(void);
#define __preempt_schedule_notrace() preempt_schedule_notrace()
#endif /* CONFIG_PREEMPT */

#endif /* _ASM_PREEMPT_H */
