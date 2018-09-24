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

struct cpumask __cpu_possible_mask;
struct cpumask __cpu_online_mask;
struct cpumask __cpu_present_mask;
struct cpumask __cpu_active_mask;

#if NR_CPUS > 1
unsigned int nr_cpu_ids;
#endif // NR_CPUS > 1

#ifdef CONFIG_SMP
unsigned long __per_cpu_offset[NR_CPUS];

DEFINE_PER_CPU_READ_MOSTLY(unsigned long, this_cpu_off);
#endif // CONFIG_SMP

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
