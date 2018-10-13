#include <string.h>
// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);

#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/inet_sock.h>
#include <net/pkt_sched.h>
#include <net/udp_tunnel.h>

#include <kernel_config.h>
#include <klee.h>

#include <device.h>
#include <socket.h>

extern struct rtnl_link_ops wg_link_ops;
extern struct inet_sock sock_ipv4_0;
extern udp_tunnel_encap_rcv_t sock_ipv4_0_receive;

/**
 * Initialize `net_device` similar to `alloc_netdev_mqs`.
 */
struct net_device *stub_alloc_netdev_mqs()
{
	unsigned int txqs = 1;
	unsigned int rxqs = 1;

	size_t device_size = sizeof(struct net_device);
	/* ensure 32-byte alignment of private area */
	device_size = ALIGN(device_size, NETDEV_ALIGN);
	device_size += sizeof(struct wg_device);
	/* ensure 32-byte alignment of whole construct */
	device_size += NETDEV_ALIGN - 1;

	struct net_device *dev = malloc(device_size);
	dev->padded = 0;

	// dev->pcpu_refcnt = alloc_percpu(int);
	// dev_addr_init(dev);
	// dev_mc_init(dev);
	// dev_uc_init(dev);
	// dev_net_set(dev, &init_net);
	dev->gso_max_size = GSO_MAX_SIZE;
	dev->gso_max_segs = GSO_MAX_SEGS;
	INIT_LIST_HEAD(&dev->napi_list);
	INIT_LIST_HEAD(&dev->unreg_list);
	INIT_LIST_HEAD(&dev->close_list);
	INIT_LIST_HEAD(&dev->link_watch_list);
	INIT_LIST_HEAD(&dev->adj_list.upper);
	INIT_LIST_HEAD(&dev->adj_list.lower);
	INIT_LIST_HEAD(&dev->ptype_all);
	INIT_LIST_HEAD(&dev->ptype_specific);
#ifdef CONFIG_NET_SCHED
	hash_init(dev->qdisc_hash);
#endif
	dev->priv_flags = IFF_XMIT_DST_RELEASE | IFF_XMIT_DST_RELEASE_PERM;

	// Call WG `setup` op for the new device.
	// Ops recorded as global by stubs in `rtnetlink.c`
	wg_link_ops.setup(dev);

	if (!dev->tx_queue_len) {
		dev->priv_flags |= IFF_NO_QUEUE;
		dev->tx_queue_len = DEFAULT_TX_QUEUE_LEN;
	}

	dev->num_tx_queues = txqs;
	dev->real_num_tx_queues = txqs;
	// netif_alloc_netdev_queues(dev);

#ifdef CONFIG_SYSFS
	dev->num_rx_queues = rxqs;
	dev->real_num_rx_queues = rxqs;
	// netif_alloc_rx_queues(dev);
#endif

	strcpy(dev->name, "wg0");
	dev->name_assign_type = NET_NAME_USER;
	dev->group = INIT_NETDEV_GROUP;
	// if (!dev->ethtool_ops) {
	//   dev->ethtool_ops = &default_ethtool_ops;
	// }

	// nf_hook_ingress_init(dev);

	return dev;
}

/**
 * Create a new link similiar to `rtnl_newlink`:
 *
 *   - Loads the kernel module if not loaded
 *   - Calls `rtnl_create_link`
 *     - Calls `alloc_netdev_mqs` to allocate the `net_device`
 *     - Notifies WG `device.c` via `setup` op
 *     - Allocates various queues
 *   - Notifies WG `device.c` via `newlink` op
 *     - Calls `register_netdevice` to install new device
 *   - Calls `rtnl_configure_link`
 *     - Sets `rtnl_link_state` to `RTNL_LINK_INITIALIZED`
 */
struct net_device *stub_rtnl_newlink()
{
	// Initialize the kernel module as if just loaded.
	init_module();

	// Initialize `net_device` similar to `alloc_netdev_mqs`.
	struct net_device *dev = stub_alloc_netdev_mqs();

	// Notify WG via `newlink` op of the succesfully allocated link.
	// Ops recorded as global by stubs in `rtnetlink.c`
	wg_link_ops.newlink(/* src_net */ NULL, dev,
			    /* tb */ NULL, /* data */ NULL, /* ext_act */ NULL);

	return dev;
}

int main(int argc, char *argv[])
{
	init_globals();

	/*
	 * $ ip link add dev wg0 type wireguard
	 *
	 * Sends RTM_NEWLINK message to kernel with:
	 *   - IFLA_IFNAME: wg0
	 *   - IFLA_INFO_KIND: wireguard
	 *
	 * Received by `rtnl_newlink`, similar to the stub above.
	 */
	struct net_device *dev = stub_rtnl_newlink();

	/*
	 * $ ip address add dev wg0 192.168.2.1 peer 192.168.2.2
	 */

	/*
	 * $ wg set wg0
	 *   listen-port 51820
	 *   private-key /path/to/private-key
	 *   peer ABCDEF...
	 *   allowed-ips 192.168.88.0/24
	 *   endpoint 209.202.254.14:8172
	 *
	 * Uses interface defined in `uapi/wiregaurd.h` and processed by
	 * `netlink.c`.
	 */

	/*
	 * $ ip address add 192.168.88.70/24 dev wg0
	 *
	 * TODO: Document these steps.
	 */

	/*
	 * $ ip link set up dev wg0
	 *
	 * ... other steps ...
	 *
	 * - Notifies WG `device.c` via `ndo_open` op
	 *   - Calls `wg_socket_init` in WG `socket.c`
	 *     - Calls `udp_sock_create` in kernel `udp_tunnel.c`
	 *     - Calls `setup_udp_tunnel_sock` in kernel `udp_tunnel.c`
	 */

	// NOTE: We assume the kernel is creating buffers correctly, with the
	// correct size, etc. The data inside the buffer is symbolic. We may
	// loosen these assumptions later on to also test what happens if the
	// kernel misbehaves.

	// Skip ahead and try forcing a socket!
	wg_socket_init(netdev_priv(dev), 12345);

	// Create a symbolic packet to receive
	struct sk_buff skb;
	klee_make_symbolic(&skb, sizeof(skb), "r_skb");

	// Initialize similar to `alloc_skb`
	// `buffer_size` is the size of the buffer.
	// The kernel defines a max size of SKB_MAX_ALLOC = PAGE_SIZE << 2 =
	// 16384. The tunnel's MTU defaults to 1420. Tracing shows the full
	// buffer size maxes out around 2304. Let's be safe and offer a bit more
	// room for now.
	size_t buffer_size = 3072;
	char *data = malloc(buffer_size);
	klee_make_symbolic(data, buffer_size, "r_data");
	// TODO: Adjust size for `skb_shared_info`?

	// Clear most `skb` metadata.
	// TODO: Relax some of these, such as `len`?
	memset(&skb, 0, offsetof(struct sk_buff, tail));
	// Records data size plus skb and skb shared structs.
	skb.truesize = SKB_TRUESIZE(buffer_size);

	// Set some basics, such as pointers, to concrete values to save query
	// time.
	// TODO: Relax some of these...?
	// Set up head, data, and tail as 0 size like `alloc_skb`.
	skb.head = data;
	skb.data = data;
	skb.tail = 0;
	skb.end = skb.tail + buffer_size;

	// For now, init these headers like `alloc_skb`.
	skb.mac_header = (typeof(skb.mac_header)) ~0U;
	skb.transport_header = (typeof(skb.transport_header)) ~0U;

	// Set symbolic head size like `skb_reserve`.
	const size_t head_size = klee_range(0, buffer_size + 1, "r_head_size");
	skb.data += head_size;
	skb.len += head_size;

	// Set symbolic data size like `skb_put`.
	const size_t data_size = klee_range(0, buffer_size + 1, "r_data_size");
	skb.tail += data_size;
	skb.len += data_size;

	// TODO: Add KLEE asserts when these sizes are bad...?

	// Set symbolic transport header (UDP).
	const size_t transport_header_offset =
		klee_range(0, buffer_size + 1, "r_transport_header_offset");
	skb.transport_header = skb.data - skb.head;
	skb.transport_header += transport_header_offset;

	// Set symbolic network header (IP).
	const size_t network_header_offset =
		klee_range(0, buffer_size + 1, "r_network_header_offset");
	skb.network_header = skb.data - skb.head;
	skb.network_header += network_header_offset;

	// Receive recorded as global by stubs in `udp_tunnel.c`
	sock_ipv4_0_receive(&sock_ipv4_0.sk, &skb);

	free(data);

	return 0;
}
