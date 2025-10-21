#include "../inc/ft_ping.h"

void init_socket(struct ping *p) {
	p->socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (p->socket == -1) {
		ping_error("%s\n", strerror(errno));
	}
	struct opt *interval = get_opt(p->opts, "interval", 0);
	struct timeval tv = {
		.tv_sec = (int)interval->val.dbl,
		.tv_usec = (interval->val.dbl - tv.tv_sec) * 1000000
	};
	size_t optlen = sizeof(tv);
	if (setsockopt(p->socket, SOL_SOCKET, SO_RCVTIMEO, &tv, optlen) == -1) {
		ping_error("%s\n", strerror(errno));
	}
}

void init_buf(struct ping *p) {
	size_t size = get_opt(p->opts, "size", 0)->val.intgr;
	p->sendbuf = malloc(size + sizeof(struct icmphdr));
	if (!p->sendbuf) {
		ping_error("%s\n", strerror(errno));
	}
	gc_add(p->sendbuf);
	p->recvbuf = malloc(size + sizeof(struct icmphdr) + sizeof (struct iphdr));
	if (!p->recvbuf) {
		ping_error("%s\n", strerror(errno));
	}
	gc_add(p->recvbuf);
}

void ping_init(struct ping *p) {
	init_socket(p);
	init_buf(p);
}
