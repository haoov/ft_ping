/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:46:33 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/12 19:05:30 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

void	set_timestamp(byte_t *data) {
	long			*ts;
	struct timeval	tv;

	ts = (long*)data;
	if (gettimeofday(&tv, NULL) == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	*ts++ = tv.tv_sec;
	*ts = tv.tv_usec;
}

void	fill_data(byte_t *data, int size) {
	int		i;
	int		ts_size;

	ts_size = sizeof(long) * 2;
	i = ts_size;
	while (i < size) {
		data[i] = i - ts_size;
		i++;
	}
	set_timestamp(data);
}

byte2_t	compute_checksum(byte_t *pkt, int size) {
	byte4_t	sum;

	while (size > 1) {
		sum += *(byte2_t*)pkt++;
		size -= 2;
	}
	if (size > 0) {
		sum += *(byte2_t*)pkt;
	}
	while (sum >> 16) {
		sum = (sum & 0xFFFF) + (sum >> 16);
	}
	return (byte2_t)~sum;
}

void	build_icmp_packet(byte_t *icmp_pkt, byte2_t seq, int size) {
	struct icmphdr	*hdr;

	hdr = (struct icmphdr*)icmp_pkt;
	hdr->type = ICMP_ECHO;
	hdr->code = 0x0;
	hdr->un.echo.id = 0x2A;
	hdr->un.echo.sequence = htons(seq);
	fill_data(icmp_pkt + ICMP_HD_SIZE, size - ICMP_HD_SIZE);
	hdr->checksum = htons(compute_checksum(icmp_pkt, ICMP_DEF_PKT_SIZE));
}

void	send_icmp_echo(struct ft_ping *ft_ping) {
	if (sendto(
			ft_ping->sockfd,
			ft_ping->icmp_pkt,
			ft_ping->pkt_size,
			0,
			&ft_ping->dst,
			sizeof(ft_ping->dst)
		) == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	ft_ping->npkt_sent++;
}
