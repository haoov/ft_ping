#include "../inc/ft_ping.h"

void ping_host(char *host, struct ping *p) {
	// TODO: ping single host with interrupt handling for host 0  
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

		double interval = get_opt(p->opts, "interval", 0)->val.dbl;
		while (true) {
			int count = get_opt(p->opts, "count", 0)->val.intgr;

			icmp_echo_request(p);
			icmp_response(p);
			++p->stats.seq;
			if (count != 0 && (int)p->stats.nsend >= count) {
				break;
			}
			usleep(interval * 1000000);
		}

		ping_stats(host, p->stats);
	}
}
