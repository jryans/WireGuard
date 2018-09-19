#ifndef _LINUX_SKBUFF_H
#define _LINUX_SKBUFF_H

/* Vigor: Trimmed down to essential includes only. */

#include <linux/socket.h>

#include <net/flow_dissector.h>

/* Vigor: Define just enough to compile. */

struct sk_buff_head {
};

struct sk_buff {
  void *head;
  void *data;
  int len;
  int protocol;
  void *cb;
  int skb_iif;
};

#endif /* _LINUX_SKBUFF_H */
