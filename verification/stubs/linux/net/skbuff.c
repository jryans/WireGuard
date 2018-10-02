#include <linux/skbuff.h>

#include <linux/stddef.h>

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
