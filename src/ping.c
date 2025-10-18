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

void create_icmp_packet() {
	struct icmp *pkt = (struct icmp *)ping.sendbuf;
	int data_size = get_opt("size", 0)->val.intgr;
	int total_size = data_size + sizeof (struct icmphdr);

	pkt->icmp_cksum = 0;
	pkt->icmp_type = ICMP_ECHO;
	pkt->icmp_code = 0;
	pkt->icmp_id = getpid();
	pkt->icmp_seq = ping.stats.seq;
	char *pattern = get_opt("pattern", 0)->val.ptr;
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

	pkt->icmp_cksum = compute_cheksum((uint16_t*)ping.sendbuf, total_size);
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
}

int receive_packet() {
	size_t buflen = get_opt("size", 0)->val.intgr + sizeof (struct icmphdr) + sizeof (struct iphdr);
	socklen_t addrlen = sizeof (struct sockaddr);
	ssize_t n;
	
	memset(ping.recvbuf, 0, buflen);
	n = recvfrom(ping.socket, ping.recvbuf, buflen, 0, (struct sockaddr*)&ping.addr, &addrlen);
	if (n == -1 && errno != EAGAIN && errno != EWOULDBLOCK) {
		ping_error("%s\n", strerror(errno));
	}
	++ping.stats.nrecv;
	return n;
}

void ping_echo_reply(int size) {
	struct ip *iphdr = (struct ip *)ping.recvbuf;
	int iphdrlen = iphdr->ip_hl * 4;
	struct icmp *pkt = (struct icmp *)(ping.recvbuf + iphdrlen);
	struct timeval send_time, recv_time;
	double rtt;
	int data_len = size - iphdrlen;

	gettimeofday(&recv_time, NULL);

	send_time = ping.stats.timing[pkt->icmp_seq % MAX_TIMING_PKT].send_time;
	
	rtt = (recv_time.tv_sec - send_time.tv_sec) * 1000.0;
	rtt += (recv_time.tv_usec - send_time.tv_usec) / 1000.0;

	ping.stats.tsum += rtt;
	ping.stats.tsumsq += rtt * rtt;

	if (ping.stats.tmin == 0 || ping.stats.tmin > rtt) {
		ping.stats.tmin = rtt;
	}
	ping.stats.tmax = ping.stats.tmax < rtt ? rtt : ping.stats.tmax;

	printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		data_len,
		inet_ntoa(ping.addr.sin_addr),
		pkt->icmp_seq,
		iphdr->ip_ttl,
		rtt
	);
}

void parse_packet(int size) {
	struct ip *iphdr = (struct ip *)ping.recvbuf;

	if (iphdr->ip_v != 4) {
		return;
	}

	ping_echo_reply(size);
}

void ping_stats(char *host) {
	struct stats s = ping.stats;
	double avg, mdev, variance, lost_pct;

	printf("--- %s ping statistics ---\n", host);

	if (ping.stats.nsend > 0) {
		lost_pct = ((double)(s.nsend - s.nrecv) / s.nsend) * 100.0;
	}
	else {
		lost_pct = 0.0;
	}

	printf ("%lu packets transmitted, %lu packets received, %.0f%% packet loss\n",
		 s.nsend,
		 s.nrecv,
		 lost_pct
	);

	if (s.nrecv > 0) {
		avg = s.tsum / s.nrecv;

		if (s.nrecv > 1) {
			variance = (s.tsumsq / s.nrecv) - (avg * avg);

			if (variance < 0.0) {
				variance = 0.0;
			}

			mdev = sqrt(variance);
		}
		else {
			mdev = 0.0;
		}
		
		printf("rtt min/avg/max/mdev = %.3f/%.3f/%.3f/%.3f ms\n",
			s.tmin,
			avg,
			s.tmax,
			mdev
		);
	}
}

void ft_ping() {
	for (int i = 0; i < ping.host_count; ++i) {
		char *host = ping.hosts[i];
		int n;

		memset(&ping.stats, 0, sizeof (ping.stats));
		ping.stats.seq = 1;
		resolve_host(host);
		printf("PING %s (%s) %d bytes of data.\n",
			host,
			inet_ntoa(ping.addr.sin_addr),
			get_opt("size", 0)->val.intgr
		);

		// Start loop
		create_icmp_packet();
		send_packet();
		n = receive_packet();
		parse_packet(n);
		sleep(1);
		++ping.stats.seq;
		// End loop

		ping_stats(host);
	}
}
