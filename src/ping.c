#include "../inc/ft_ping.h"

extern sig_atomic_t interrupt;

void ping_host(struct ping *p) {
	int count = get_opt(p->opts, "count", 0)->val.intgr;
	double interval = get_opt(p->opts, "interval", 0)->val.dbl;
	double timeout = get_opt(p->opts, "timeout", 0)->val.dbl;
	struct timeval start, now;

	if (timeout > 0) {
		gettimeofday(&start, NULL);
	}

	while (true) {
		if (interrupt) {
			return;
		}
		icmp_echo_request(p);
		icmp_response(p);
		++p->stats.seq;
		if (count != 0 && (int)p->stats.nsend >= count) {
			return;
		}
		if (timeout > 0) {
			gettimeofday(&now, NULL);
			double elapsed = (now.tv_sec - start.tv_sec)
				+ (now.tv_usec - start.tv_usec) / 1000000.0;
			if (elapsed >= timeout) {
				return;
			}
		}
		if (!interrupt) {
			usleep(interval * 1000000);
		}
	}
}

void ft_ping(struct ping *p) {
	for (int i = 0; i < p->host_count; ++i) {
		char *host = p->hosts[i];

		memset(&p->stats, 0, sizeof (p->stats));
		p->stats.seq = 1;
		p->stats.tmin = -1.0;
		resolve_host(p, host);
		int data_size = get_opt(p->opts, "size", 0)->val.intgr;
		int total_size = data_size + (int)(sizeof(struct icmphdr) + sizeof(struct iphdr));
		printf("PING %s (%s): %d(%d) bytes of data.\n",
			host,
			inet_ntoa(p->addr.sin_addr),
			data_size,
			total_size
		);

		ping_host(p);

		ping_stats(host, p->stats);
	}
}
