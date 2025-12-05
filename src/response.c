#include "ft_ping.h"

static const char *const dest_unreach_msgs[16] = {
	[ICMP_UNREACH_NET] = "Destination Net Unreachable",
	[ICMP_UNREACH_HOST] = "Destination Host Unreachable",
	[ICMP_UNREACH_PROTOCOL] = "Destination Protocol Unreachable",
	[ICMP_UNREACH_PORT] = "Destination Port Unreachable",
	[ICMP_UNREACH_NEEDFRAG] = "Frag needed and DF set (MTU = %d)",
	[ICMP_UNREACH_SRCFAIL] = "Source Route Failed",
	[ICMP_UNREACH_NET_UNKNOWN] = "Destination Network Unknown",
	[ICMP_UNREACH_HOST_UNKNOWN] = "Destination Host Unknown",
	[ICMP_UNREACH_ISOLATED] = "Source Host Isolated",
	[ICMP_UNREACH_NET_PROHIB] = "Destinatio Network Prohibited",
	[ICMP_UNREACH_HOST_PROHIB] = "Destination Host Prohibited",
	[ICMP_UNREACH_TOSNET] = "Network Unreachable for Type of Service",
	[ICMP_UNREACH_TOSHOST] = "Host Unreachable for Type of Service",
	[ICMP_UNREACH_FILTER_PROHIB] = "Communication Administratively Prohibited",
	[ICMP_UNREACH_HOST_PRECEDENCE] = "Host Precedence violation",
	[ICMP_UNREACH_PRECEDENCE_CUTOFF] = "Precedence cutoff"
};

static const char *const redirect_msgs[4] = {
	[ICMP_REDIRECT_NET] = "Redirect Network",
	[ICMP_REDIRECT_HOST] = "Redirect Host",
	[ICMP_REDIRECT_TOSNET] = "Redirect Type of Service and Network",
	[ICMP_REDIRECT_TOSHOST] = "Redirect Type of Service and Host"
};

int receive_icmp_packet(struct ping *p) {
	size_t buflen = get_opt(p->opts, "size", 0)->val.intgr + sizeof (struct icmphdr) + sizeof (struct iphdr);
	socklen_t addrlen = sizeof (struct sockaddr);
	ssize_t n;
	
	memset(p->recvbuf, 0, buflen);
	n = recvfrom(p->socket, p->recvbuf, buflen, 0, (struct sockaddr*)&p->addr, &addrlen);
	if (n == -1) {
		switch (errno) {
			case EAGAIN:
				printf("Request timeout for icmp_seq %d\n", p->stats.seq);
				break;
			case EINTR:
				return n;
			default:
				ping_error("%s\n", strerror(errno));
		}
	}
	++p->stats.nrecv;
	return n;
}

int icmp_echo_reply(struct ping *p, int size) {
	struct ip *ip = (struct ip *)p->recvbuf;
	struct in_addr dest = ip->ip_dst;
	int iphdrlen = ip->ip_hl * 4;
	struct icmp *pkt = (struct icmp *)(p->recvbuf + iphdrlen);
	struct timeval send_time, recv_time;
	double rtt;
	int data_len = size - iphdrlen;

	gettimeofday(&recv_time, NULL);

	if (compute_cheksum((uint16_t *)pkt, data_len) != 0) {
		printf("%d bytes from %s: icmp_seq=%d (BAD CHECKSUM)\n",
			data_len,
			inet_ntoa(dest),
			pkt->icmp_seq
		);
		return -1;
	}

	char *pattern = get_opt(p->opts, "pattern", 0)->val.ptr;
	if (pattern) {
		size_t plen = strlen(pattern);
		char *data = (char *)pkt->icmp_data;
		for (int i = 0; i < (int)(data_len - sizeof (struct icmphdr)); ++i) {
			if (data[i] != pattern[i % plen]) {
				printf("wrong data byte #%d should be 0x%02X but was 0x%02X\n",
					i, pattern[i % plen], data[i]
				);
				return -1;
			}
		}
	}

	send_time = p->stats.timing[pkt->icmp_seq % MAX_TIMING_PKT].send_time;
	
	rtt = (recv_time.tv_sec - send_time.tv_sec) * 1000.0;
	rtt += (recv_time.tv_usec - send_time.tv_usec) / 1000.0;

	p->stats.tsum += rtt;
	p->stats.tsumsq += rtt * rtt;

	if (p->stats.tmin == 0 || p->stats.tmin > rtt) {
		p->stats.tmin = rtt;
	}
	p->stats.tmax = p->stats.tmax < rtt ? rtt : p->stats.tmax;

	if (get_opt(p->opts, "quiet", 0)->val.intgr) {
		return 0;
	}

	printf("%d bytes from %s: icmp_seq=%u ttl=%d time=%.3f ms\n",
		data_len,
		inet_ntoa(dest),
		pkt->icmp_seq,
		ip->ip_ttl,
		rtt
	);

	return 0;
}

void icmp_dest_unreachable(struct icmp *icmp) {
	int code = icmp->icmp_code;
	struct ip *ip = (struct ip *)icmp->icmp_data;
	struct in_addr dest = ip->ip_dst;

	printf("From %s: ", inet_ntoa(dest));

	if (code == ICMP_UNREACH_NEEDFRAG) {
		printf(dest_unreach_msgs[code], ntohs(icmp->icmp_nextmtu));
	}
	else if (code < 16) {
		printf("%s\n",dest_unreach_msgs[code]);
	}
	else {
		printf("Destination Unreachable, Unknown code %d\n", code);
	}
}

void icmp_time_exceeded(struct icmp *icmp) {
	int code = icmp->icmp_code;
	struct ip *ip = (struct ip *)icmp->icmp_data;
	struct in_addr dest = ip->ip_dst;

	printf("From %s: icmp_seq=%d ",
		inet_ntoa(dest),
		icmp->icmp_seq
	);
	if (code == 1) {
		printf("Fragment reassembly time exceeded\n");
	}
	else if (code == 0){
		printf("Time to live exceeded\n");
	}
	else {
		printf("Time exceede, Unknown code: %d\n", code);
	}
}

void icmp_redirect(struct icmp *icmp) {
	int code = icmp->icmp_code;
	struct in_addr *gateway = (struct in_addr *)&icmp->icmp_gwaddr;

	if (code < 4) {
		printf("%s ", redirect_msgs[code]);
	}
	else {
		printf("Redirect, Unknown code: %d ", code);
	}
	
	printf("(New address %s)\n", inet_ntoa(*gateway));
}

void icmp_source_quench(struct icmp *icmp) {
	struct ip *ip = (struct ip *)icmp->icmp_data;
	struct in_addr dest = ip->ip_dst;

	printf("Source Quench from %s\n", inet_ntoa(dest));
}

void icmp_response(struct ping *p) {
	int n;

	n = receive_icmp_packet(p);

	struct ip *iphdr = (struct ip *)p->recvbuf;

	if (iphdr->ip_v != 4) {
		return;
	}
	struct icmp *icmp = (struct icmp*)(p->recvbuf + iphdr->ip_hl * 4);
	switch (icmp->icmp_type) {
		case ICMP_UNREACH:
			icmp_dest_unreachable(icmp);
			return;
		case ICMP_TIMXCEED:
			icmp_time_exceeded(icmp);
			return;
		case ICMP_SOURCEQUENCH:
			icmp_source_quench(icmp);
			return;
		case ICMP_REDIRECT:
			icmp_redirect(icmp);
			[[fallthrough]];
		case ICMP_ECHOREPLY:
			icmp_echo_reply(p, n);
		default:
			break;
	}
}
