/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:24:01 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 12:06:36 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

extern struct ping	ping;

double compute_rttdev(double rtts[MAXRTTVAL], double avg, int npkt_sent)
{
	double	calc;

	calc = 0;
	for (int i = 0; rtts[i] && i < MAXRTTVAL; ++i)
	{
		calc += powf(rtts[i] - avg, 2.0);
	}
	return sqrtf((1.0 / npkt_sent) * calc);
}

void ping_stats()
{
	double	rttdev, rttavg;

	printf("--- %s ping statistics ---\n", ping.p_host);
	printf("%d packets transmitted, %d packets received, %.0f%% packet loss\n",
			ping.npkt_sent,
			ping.npkt_recv,
			(1.0 - (double)(ping.npkt_recv / ping.npkt_sent)) * 100.0);
	if (ping.npkt_recv)
	{
		rttavg = reducef(ping.rtts, MAXRTTVAL) / ping.npkt_sent;
		rttdev = compute_rttdev(ping.rtts, rttavg, ping.npkt_sent);
		printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n",
				ping.rttmin,
				rttavg,
				ping.rttmax,
				rttdev);
	}
}

double get_rtt(uint8_t *data)
{
	struct timeval	now, *then;
	useconds_t		n, t;

	gettimeofday(&now, NULL);
	then = (struct timeval*)data;
	n = now.tv_sec * 1000000 + now.tv_usec;
	t = then->tv_sec * 1000000 + then->tv_usec;
	return (n - t) / 1000.0;
}

void reply_stats(uint8_t *buf, ssize_t size)
{
	struct iphdr	*iphdr;
	struct icmphdr	*icmphdr;
	int				iphdr_len;
	double			rtt;

	if (size == -1)
	{
		printf("%s: %s\n", ping.p_host, ERR_TIMEO);
		return;
	}
	else if ((size_t)size < sizeof(struct iphdr))
	{
		printf("%s: %s\n", ping.p_host, ERR_SMALL);
		return;
	}
	iphdr = (struct iphdr*)buf;
	iphdr_len = iphdr->ihl * sizeof(int);
	icmphdr = (struct icmphdr*)(buf + iphdr_len);
	rtt = get_rtt(buf + iphdr_len + ICMP_HD_SIZE);
	ping.rttmax = rtt > ping.rttmax ? rtt : ping.rttmax;
	if (ping.seq == 1)
	{
		ping.rttmin = rtt;
	}
	ping.rttmin = rtt < ping.rttmin ? rtt : ping.rttmin;
	ping.rtts[ping.seq - 1] = rtt;
	printf("%ld bytes from %s: icmp_seq=%d ttl=%d time=%.2f ms\n",
			size - iphdr_len,
			ping.p_host,
			ntohs(icmphdr->un.echo.sequence),
			iphdr->ttl,
			rtt);
}
