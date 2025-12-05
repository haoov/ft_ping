#include "../inc/ft_ping.h"

extern sig_atomic_t interrupt;

void ping_host(struct ping *p) {
	int count = get_opt(p->opts, "count", 0)->val.intgr;
	double interval = get_opt(p->opts, "interval", 0)->val.dbl;

	while (true) {

		if (interrupt && p->stats.seq > 1) {
			return;
		}
		icmp_echo_request(p);
		icmp_response(p);
		++p->stats.seq;
		if (count != 0 && (int)p->stats.nsend >= count) {
			return;
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
		resolve_host(p, host);
		printf("PING %s (%s) %d bytes of data.\n",
			host,
			inet_ntoa(p->addr.sin_addr),
			get_opt(p->opts, "size", 0)->val.intgr
		);

		ping_host(p);

		ping_stats(host, p->stats);
	}
}
