/*
 * Vigor: Work queue functions replaced with simple versions since we only
 * simulate a single core.
 */

#include <linux/workqueue.h>

#include <linux/types.h>
#include <asm/bitops.h>

void __queue_work(int cpu, struct workqueue_struct *wq,
                  struct work_struct *work)
{
	// TODO: Use an event queue so this happens async...
	work->func(work);
}

bool queue_work_on(int cpu, struct workqueue_struct *wq,
                   struct work_struct *work)
{
	bool ret = false;

	if (!test_and_set_bit(WORK_STRUCT_PENDING_BIT, work_data_bits(work))) {
		__queue_work(cpu, wq, work);
		ret = true;
	}

	return ret;
}
