#ifndef _NET_NET_NAMESPACE_H
#define _NET_NET_NAMESPACE_H

/* Vigor: Define just enough to compile. */

struct net {
  struct {
    bool sysctl_tcp_l3mdev_accept;
  } ipv4;
};

#endif /* _NET_NET_NAMESPACE_H */
