#include "../inc/ft_ping.h"

extern struct ping ping;

void init_socket() {
	ping.socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (ping.socket == -1) {
		ping_error("%s\n", strerror(errno));
	}
	struct opt *interval = get_opt("interval", 0);
	struct timeval tv = {
		.tv_sec = (int)interval->val.dbl,
		.tv_usec = (interval->val.dbl - tv.tv_sec) * 1000000
	};
	size_t optlen = sizeof(tv);
	if (setsockopt(ping.socket, SOL_SOCKET, SO_RCVTIMEO, &tv, optlen) == -1) {
		ping_error("%s\n", strerror(errno));
	}
}

void init_buf() {
	size_t size = get_opt("size", 0)->val.intgr;
	ping.sendbuf = malloc(size + sizeof(struct icmphdr));
	if (!ping.sendbuf) {
		ping_error("%s\n", strerror(errno));
	}
}
