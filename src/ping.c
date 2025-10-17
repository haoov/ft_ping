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

	// Debug
	printf(
		"Host: %s Address: %s\n",
		host,
		inet_ntoa(((struct sockaddr_in)ping.addr).sin_addr)
	);
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

void create_icmp_packet(uint16_t seq) {
	struct icmp *pkt = (struct icmp *)ping.sendbuf;
	int size = get_opt("size", 0)->val.intgr;

	pkt->icmp_cksum = 0;
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_code = 0;
	pkt->icmp_id = getpid();
	pkt->icmp_seq = seq;
	char *pattern = get_opt("pattern", 0)->val.ptr;
	if (pattern) {
		int i = 0;
		size_t len = strlen(pattern);

		if ((int)len > size) {
			memcpy(pkt->icmp_data, pattern, size);
		}
		else {
			while (i < size) {
				memcpy(pkt->icmp_data + i, pattern, len);
				i += len;
			}
		}
	}
	else {
		memset(pkt->icmp_data, 0x42, size - sizeof(struct icmphdr));
	}

	pkt->icmp_cksum = compute_cheksum((uint16_t*)ping.sendbuf, size);
}

void send_packet() {
	size_t buflen = get_opt("size", 0)->val.intgr + sizeof (struct icmphdr);
	socklen_t addrlen = sizeof (struct sockaddr);
	ssize_t n;
	struct timeval *time = &ping.stats.timing[ping.stats.seq % MAX_TIMING_PKT].send_time;

	gettimeofday(time, NULL);
	n = sendto(ping.socket, ping.sendbuf, buflen, 0, (struct sockaddr*)&ping.addr, addrlen);
	if (n == -1) {
		ping_error("%s\n", strerror(errno));
	}

	++ping.stats.nsend;

	// Debug
	struct icmp *pkt = (struct icmp*)ping.sendbuf;
	int size = get_opt("size", 0)->val.intgr;
	printf("%zd bytes sent\n", n);

	printf("icmp packet sent:\n");
	printf("\ttype: %d\n", pkt->icmp_type);
	printf("\tcode: %d\n", pkt->icmp_code);
	printf("\tid: %d\n", pkt->icmp_id);
	printf("\tseq: %d\n", pkt->icmp_seq);
	printf("\tdata: ");
	for (int i = 0; i < size; ++i) {
		printf("%c", pkt->icmp_data[i]);
	}
	printf("\n\tcheck_sum: %d\n", pkt->icmp_cksum);
}

void receive_packet() {
	size_t buflen = get_opt("size", 0)->val.intgr + sizeof (struct icmphdr) + sizeof (struct iphdr);
	socklen_t addrlen = sizeof (struct sockaddr);
	ssize_t n;
	
	memset(ping.recvbuf, 0, buflen);
	n = recvfrom(ping.socket, ping.recvbuf, buflen, 0, (struct sockaddr*)&ping.addr, &addrlen);
	if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		ping_error("%s\n", strerror(errno));
	}
	printf("%zd bytes received\n", n);

	// Debug
	int size = get_opt("size", 0)->val.intgr;
	struct icmp *pkt = (struct icmp*)(ping.recvbuf + sizeof (struct iphdr));

	printf("icmp packet received:\n");
	printf("\ttype: %d\n", pkt->icmp_type);
	printf("\tcode: %d\n", pkt->icmp_code);
	printf("\tid: %d\n", pkt->icmp_id);
	printf("\tseq: %d\n", pkt->icmp_seq);
	printf("\tdata: ");
	for (int i = 0; i < size; ++i) {
		printf("%c", pkt->icmp_data[i]);
	}
	printf("\n\tcheck_sum: %d\n", pkt->icmp_cksum);
}

void ft_ping() {
	for (int i = 0; i < ping.host_count; ++i) {
		char *host = ping.hosts[i];

		ping.stats.seq = 0;
		ping.stats.nsend = 0;
		ping.stats.nrecv = 0;
		resolve_host(host);
		create_icmp_packet(ping.stats.seq + 1);
		send_packet();
		receive_packet();
		sleep(1);
	}
}
