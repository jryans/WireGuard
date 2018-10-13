#include <linux/notifier.h>

int register_netdevice_notifier(struct notifier_block *nb)
{
	return 0;
}

int unregister_netdevice_notifier(struct notifier_block *nb)
{
	return 0;
}

int register_netdevice(struct net_device *dev)
{
	// TODO: Emit NETDEV_REGISTER to notifier registered above.
	// This appears to be only a minor administrative detail.
	return 0;
}

void synchronize_net()
{
}