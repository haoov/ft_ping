/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping_reply.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/13 11:06:10 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/13 18:36:11 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

useconds_t compute_sleep_time(struct timeval start, struct timeval end) {
	useconds_t	s, e, time;

	s = start.tv_sec * 1000000 + start.tv_usec;
	e = end.tv_sec * 1000000 + end.tv_usec;
	time = 1000000 - (e - s);
	return time ? time : 0;
}

ssize_t recv_icmp_reply(struct ping *ping) {
	ssize_t			size;
	socklen_t		addr_len;

	addr_len = sizeof(struct sockaddr);
	size = recvfrom(
		ping->sockfd,
		ping->icmp_pkt,
		ping->pkt_size + IP_MAX_HD_SIZE,
		0,
		&ping->dst,
		&addr_len
	);
	if (size == -1 && errno != EAGAIN) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	return size;
}

void ping_reply(struct ping *ping) {
	ssize_t				nbyte_recv;
	struct timeval		start, end;

	memset(&end, 0, sizeof(struct timeval));
	gettimeofday(&start, NULL);
	nbyte_recv = recv_icmp_reply(ping);
	if (nbyte_recv == -1) {
		ping->stime = 0;
	}
	else if (nbyte_recv) {
		++ping->npkt_recv;
		gettimeofday(&end, NULL);
		ping->stime = compute_sleep_time(start, end);
	}
	reply_stats(ping, nbyte_recv);
}
