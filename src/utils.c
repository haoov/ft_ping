#include "ft_ping.h"

void resolve_host(struct ping *p, const char *host) {
	int flags = AI_ADDRCONFIG;
	if (get_opt(p->opts, "numeric", 0)->val.intgr == true) {
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
	memcpy(&p->addr, res->ai_addr, sizeof(struct sockaddr));
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

void print_help() {
	printf("Usage: ft_ping [options] [<hosts>]\n");
	printf("Options:\n");
	printf("  -c, --count=NUMBER        Stop after sending NUMBER packets.\n");
	printf("  -i, --interval=NUMBER     Wait NUMBER seconds between sending each packet.\n");
	printf("  -n, --numeric             Do not resolve host addresses\n");
	printf("  -v, --verbose             Verbose output.\n");
	printf("  -w, --timeout=NUMBER      Time to wait for a response, in seconds.\n");
	printf("  -W, --linger=STRING       Time to wait before closing the socket, in seconds.\n");
	printf("  -?, --help                Display this help and exit.\n");
	printf("  -V, --version             Output version information and exit.\n");
	printf("  -p, --pattern=STRING      Use the specified pattern for the payload data.\n");
	printf("  -q, --quiet               Quiet output.\n");
	printf("  -s, --size=NUMBER         Specify the number of data bytes to be sent.\n");
}

void ping_no_args(struct ping *p) {
	if (get_opt(p->opts, "help", 0)->val.intgr) {
		print_help();
		exit(EXIT_SUCCESS);
	}
	if (get_opt(p->opts, "version", 0)->val.intgr) {
		printf("ft_ping version 1.0.0\n");
		exit(EXIT_SUCCESS);
	}

	ping_error("missing host operand\n");
}
