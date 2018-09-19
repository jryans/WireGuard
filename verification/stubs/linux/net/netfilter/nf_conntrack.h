#ifndef _NF_CONNTRACK_H
#define _NF_CONNTRACK_H

/* Vigor: Trimmed down to essential includes only. */

#include <linux/netfilter/nf_conntrack_common.h>

/* Vigor: Define just enough to compile. */

struct tuplehash {
  struct {
    struct {
      struct {
        int ip;
        int in6;
      } u3;
    } src;
  } tuple;
};

struct nf_conn {
  struct tuplehash tuplehash[1];
};

#endif /* _NF_CONNTRACK_H */
