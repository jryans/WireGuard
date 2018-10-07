#include <linux/skbuff.h>

#include <linux/stddef.h>

// TODO: Compare these to full versions in the kernel and expand supported paths
// for framentation, etc. if needed.

void consume_skb(struct sk_buff *skb)
{
}

void *__pskb_pull_tail(struct sk_buff *skb, int delta)
{
  return NULL;
}

void *skb_pull(struct sk_buff *skb, unsigned int len)
{
  return skb_pull_inline(skb, len);
}

void *skb_push(struct sk_buff *skb, unsigned int len)
{
  skb->data -= len;
  skb->len  += len;
  if (unlikely(skb->data<skb->head))
    skb_under_panic(skb, len, __builtin_return_address(0));
  return skb->data;
}

void skb_queue_tail(struct sk_buff_head *list, struct sk_buff *newsk)
{
  __skb_queue_tail(list, newsk);
}
