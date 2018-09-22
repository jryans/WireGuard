#include <net/rtnetlink.h>

int rtnl_link_register(struct rtnl_link_ops *ops)
{
  return 0;
}

void rtnl_link_unregister(struct rtnl_link_ops *ops)
{
}
