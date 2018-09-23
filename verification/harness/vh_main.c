#include <string.h>

#include <linux/module.h>

#include "../../device.h"
#include "../../socket.h"

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

  // Skip ahead and try forcing a socket!
  struct wg_device wg;
  // TODO: Remove this once `setup` is called
  memset(&wg, 0, sizeof(wg));

  wg_socket_init(&wg, 12345);

  return 0;
}
