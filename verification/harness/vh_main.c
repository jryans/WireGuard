#include <string.h>
// #include <stdlib.h> for malloc
extern void *malloc(size_t __size);

#include <linux/module.h>
#include <linux/skbuff.h>
#include <net/udp_tunnel.h>
#include <net/inet_sock.h>

#include <kernel_config.h>
#include <klee.h>

#include "../../device.h"
#include "../../socket.h"

extern struct inet_sock sock_ipv4_0;
extern udp_tunnel_encap_rcv_t sock_ipv4_0_receive;

struct wg_device wg0;

int main(int argc, char* argv[])
{
  /*
   * $ ip link add dev wg0 type wireguard
   *
   * Sends RTM_NEWLINK message to kernel with:
   *   - IFLA_IFNAME: wg0
   *   - IFLA_INFO_KIND: wireguard
   *
   * Received by `rtnl_newlink` which:
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
  init_module();

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
   * Uses interface defined in `uapi/wiregaurd.h` and processed by `netlink.c`.
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

  /* ... various init steps ... */

  // NOTE: We assume the kernel is creating buffers correctly, with the correct
  // size, etc. The data inside the buffer is symbolic. We may loosen these
  // assumptions later on to also test what happens if the kernel misbehaves.

  // Skip ahead and try forcing a socket!
  // TODO: Remove this once `setup` is called, or make symbolic...?
  memset(&wg0, 0, sizeof(wg0));

  wg_socket_init(&wg0, 12345);

  // Create a symbolic packet to receive
  struct sk_buff skb;
  klee_make_symbolic(&skb, sizeof(skb), "r_skb");

  // Initialize similar to `alloc_skb`
  // `alloc_size` is the size of the buffer.
  // The kernel defines a max size of SKB_MAX_ALLOC = PAGE_SIZE << 2 = 16384.
  // The tunnel's MTU defaults to 1420. Tracing shows the full buffer size maxes
  // out around 2304. Let's be safe and offer a bit more room for now.
  size_t alloc_size = 3072;
  char *data = malloc(alloc_size);
  klee_make_symbolic(data, alloc_size, "r_data");
  // TODO: Adjust size for `skb_shared_info`?

  // Clear most `skb` metadata.
  // TODO: Relax some of these, such as `len`?
  memset(&skb, 0, offsetof(struct sk_buff, tail));
  // Records data size plus skb and skb shared structs.
  skb.truesize = SKB_TRUESIZE(alloc_size);

  // Set some basics, such as pointers, to concrete values to save query time.
  // TODO: Relax some of these...?
  // Set up head, data, and tail as 0 size like `alloc_skb`.
  skb.head = data;
  skb.data = data;
  skb.tail = 0;
  skb.end = skb.tail + alloc_size;

  // For now, init these headers like `alloc_skb`.
  skb.mac_header = (typeof(skb.mac_header))~0U;
  skb.transport_header = (typeof(skb.transport_header))~0U;

  // Set symbolic head size like `skb_reserve`.
  const size_t head_size =
    klee_range(0, SKB_MAX_ALLOC + 1, "r_head_size");
  skb.data += head_size;
  skb.len += head_size;

  // Set symbolic data size like `skb_put`.
  const size_t data_size = klee_range(0, SKB_MAX_ALLOC + 1, "r_data_size");
  skb.tail += data_size;
  skb.len += data_size;

  // TODO: Add KLEE asserts when these sizes are bad...?

  // Set symbolic transport header (UDP).
  const size_t transport_header_offset =
    klee_range(0, SKB_MAX_ALLOC + 1, "r_transport_header_offset");
  skb.transport_header = skb.data - skb.head;
  skb.transport_header += transport_header_offset;

  // Set symbolic network header (IP).
  const size_t network_header_offset =
    klee_range(0, SKB_MAX_ALLOC + 1, "r_network_header_offset");
  skb.network_header = skb.data - skb.head;
  skb.network_header += network_header_offset;

  // Receive recorded as global by stubs in `udp_tunnel.c`
  sock_ipv4_0_receive(&sock_ipv4_0.sk, &skb);

  free(data);

  return 0;
}
