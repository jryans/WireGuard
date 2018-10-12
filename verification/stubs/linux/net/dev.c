#include <linux/notifier.h>

int register_netdevice_notifier(struct notifier_block *nb)
{
	return 0;
}

int unregister_netdevice_notifier(struct notifier_block *nb)
{
	return 0;
}

void synchronize_net()
{
}