#include <net/udp_tunnel.h>

#include <linux/net.h>
#include <net/inet_sock.h>

struct inet_sock sock_ipv4_0;
struct socket socket_ipv4_0 = {
	.sk = &sock_ipv4_0.sk,
};
udp_tunnel_encap_rcv_t sock_ipv4_0_receive;

struct inet_sock sock_ipv6_0;
struct socket socket_ipv6_0 = {
	.sk = &sock_ipv6_0.sk,
};

int udp_sock_create4(struct net *net, struct udp_port_cfg *cfg,
		     struct socket **sockp)
{
	sock_ipv4_0.sk.sk_socket = &socket_ipv4_0;
	*sockp = &socket_ipv4_0;
	return 0;
}

int udp_sock_create6(struct net *net, struct udp_port_cfg *cfg,
		     struct socket **sockp)
{
	sock_ipv6_0.sk.sk_socket = &socket_ipv6_0;
	*sockp = &socket_ipv6_0;
	return 0;
}

void setup_udp_tunnel_sock(struct net *net, struct socket *sock,
			   struct udp_tunnel_sock_cfg *cfg)
{
	sock_ipv4_0_receive = cfg->encap_rcv;
	sock->sk->sk_user_data = cfg->sk_user_data;
}

void udp_tunnel_sock_release(struct socket *sock)
{
}