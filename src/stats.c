#include "ft_ping.h"

void ping_stats(char *host, struct stats s) {
	double avg, mdev, variance, lost_pct;

	printf("--- %s ping statistics ---\n", host);

	if (s.nsend > 0) {
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
