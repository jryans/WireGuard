#ifndef _NET_SOCK_H
#define _NET_SOCK_H

/* Vigor: Define just enough to compile. */

struct sock {
  unsigned char sk_ipv6only:1;
  unsigned short sk_family;
#if IS_ENABLED(CONFIG_IPV6)
  struct in6_addr sk_v6_daddr;
  struct in6_addr sk_v6_rcv_saddr;
#endif
};

#endif /* _NET_SOCK_H */
