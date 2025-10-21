#include "ft_ping.h"

void create_icmp_packet(struct ping *p) {
	struct icmp *pkt = (struct icmp *)p->sendbuf;
	int data_size = get_opt(p->opts, "size", 0)->val.intgr;
	int total_size = data_size + sizeof (struct icmphdr);

	pkt->icmp_cksum = 0;
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_code = 0;
	pkt->icmp_id = getpid();
	pkt->icmp_seq = p->stats.seq;
	char *pattern = get_opt(p->opts, "pattern", 0)->val.ptr;
	if (pattern) {
		int i = 0;
		size_t len = strlen(pattern);

		if ((int)len > data_size) {
			memcpy(pkt->icmp_data, pattern, data_size);
		}
		else {
			while (i < data_size) {
				memcpy(pkt->icmp_data + i, pattern, len);
				i += len;
			}
		}
	}
	else {
		memset(pkt->icmp_data, 0x42, data_size);
	}

	pkt->icmp_cksum = compute_cheksum((uint16_t*)p->sendbuf, total_size);
}

void send_icmp_packet(struct ping *p) {
	size_t buflen = get_opt(p->opts, "size", 0)->val.intgr + sizeof (struct icmphdr);
	socklen_t addrlen = sizeof (struct sockaddr);
	struct timeval *time;
	ssize_t n;

	time = &p->stats.timing[p->stats.seq % MAX_TIMING_PKT].send_time;
	gettimeofday(time, NULL);
	n = sendto(
		p->socket,
		p->sendbuf,
		buflen,
		0,
		(struct sockaddr*)&p->addr,
		addrlen
	);
	if (n == -1) {
		ping_error("%s\n", strerror(errno));
	}

	++p->stats.nsend;
}

void icmp_echo_request(struct ping *p) {
	create_icmp_packet(p);
	send_icmp_packet(p);
}
