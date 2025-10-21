#include "../inc/ft_ping.h"

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

		// Start loop
		icmp_echo_request(p);
		icmp_response(p);
		sleep(1);
		++p->stats.seq;
		// End loop

		ping_stats(host, p->stats);
	}
}
