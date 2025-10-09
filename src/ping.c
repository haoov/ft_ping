#include "../inc/ft_ping.h"

extern struct ping ping;

void resolve_host(const char *host) {
	int flags = AI_ADDRCONFIG;
	if (get_opt("numeric", 0)->val.intgr == true) {
		flags |= AI_NUMERICHOST;
	}
	struct addrinfo hints = {
		.ai_family = PF_INET,
		.ai_socktype = SOCK_RAW,
		.ai_protocol = IPPROTO_ICMP,
		.ai_flags = flags,
	};
	struct addrinfo *res = NULL;
	if (getaddrinfo(host, NULL, &hints, &res) == -1) {
		ping_error("%s\n", strerror(errno));
	}
	if (!res) {
		ping_error("unknown host `%s`\n", host);
	}
	memcpy(&ping.addr, res->ai_addr, sizeof(struct sockaddr));
	freeaddrinfo(res);
}

uint16_t compute_cheksum(uint16_t *addr, int count) {
	uint64_t sum = 0;
	while (count > 1) {
		sum += *addr++;
		count -= 2;
	}
	
	if (count > 0) {
		sum += *(uint8_t *)addr;
	}

	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);

	return (~sum);
}

void ft_ping() {
	struct strlist *host = ping.hosts;
	while (host) {
		resolve_host(host->str);
		printf(
			"Host: %s Address: %s\n",
			host->str,
			inet_ntoa(((struct sockaddr_in)ping.addr).sin_addr)
		);
		host = host->next;
	}
}
