/**
 * Define various globals expected from the kernel.
 */

#include <asm/preempt.h>
#include <linux/cpumask.h>
#include <linux/in6.h>
#include <linux/jiffies.h>
#include <linux/percpu-defs.h>
#include <linux/workqueue.h>
#include <net/addrconf.h>
#include <net/flow_dissector.h>

/* Preemption */

DEFINE_PER_CPU(int, __preempt_count);

/* Multiple CPUs / SMP */

struct cpumask __cpu_possible_mask = { CPU_BITS_CPU0 };
struct cpumask __cpu_online_mask = { CPU_BITS_CPU0 };
struct cpumask __cpu_present_mask = { CPU_BITS_CPU0 };
struct cpumask __cpu_active_mask = { CPU_BITS_CPU0 };

/* Memory Management */

unsigned long totalram_pages;

/* Network */

struct flow_dissector flow_keys_dissector;
const struct in6_addr in6addr_any;
const struct ipv6_stub *ipv6_stub __read_mostly;

/* Jiffies */

unsigned long volatile __cacheline_aligned_in_smp __jiffy_arch_data jiffies;

/* Work Queue */

struct workqueue_struct *system_power_efficient_wq;

void init_globals()
{
	// ratelimiter.c uses this values to determine a good hash table size.
	// If the total RAM is above 1 GiB, it uses a constant size.  If not,
	// the size is computed relative to the total RAM.  Allow total RAM to
	// range from 128 KiB (the minimum safe value for the size computation)
	// to 2 GiB.
	totalram_pages =
		klee_range(1U << (17 - PAGE_SHIFT), (1U << (31 - PAGE_SHIFT)),
			   "totalram_pages");
}
