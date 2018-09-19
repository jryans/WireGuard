#ifndef _LINUX_NETDEVICE_H
#define _LINUX_NETDEVICE_H

#include <net/net_namespace.h>

/* Vigor: Define just enough to compile. */

struct net_device {
  char name[1];
};

struct napi_struct {
};

#endif /* _LINUX_NETDEVICE_H */
