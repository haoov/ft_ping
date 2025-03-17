/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   icmp_request.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 10:46:33 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/17 22:30:22 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

void	set_timestamp(uint8_t *data)
{
	long			*ts;
	struct timeval	tv;

	ts = (long*)data;
	if (gettimeofday(&tv, NULL) == -1)
	{
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	*ts++ = tv.tv_sec;
	*ts = tv.tv_usec;
}

void	fill_data(uint8_t *data, int size)
{
	int		i;
	int		ts_size;

	ts_size = sizeof(long) * 2;
	i = ts_size;
	while (i < size)
	{
		data[i] = i - ts_size;
		i++;
	}
	set_timestamp(data);
}

/*
 * Compute packet checksum.
 * */
uint16_t	compute_checksum(uint8_t *pkt, int size)
{
	uint32_t	sum;

	sum = 0;
	/* Add 16bits words values in a 32bits accumulator to store carries */
	while (size > 1)
	{
		sum += *(uint16_t*)pkt++;
		size -= 2;
	}
	/* If odd number of words add the last one */
	if (size > 0)
	{
		sum += *(uint16_t*)pkt;
	}
	/* Fold 32bits sum into 16bits */
	while (sum >> 16)
	{
		sum = (sum & 0xFFFF) + (sum >> 16);
	}
	/* Returns 1's complement */
	return (uint16_t)~sum;
}

void	build_icmp_packet(uint8_t *icmp_pkt, int size, uint16_t seq)
{
	struct icmphdr	*hdr;

	hdr = (struct icmphdr*)icmp_pkt;
	hdr->type = ICMP_ECHO;
	hdr->code = 0x0;
	hdr->un.echo.id = htons(getpid() & 0xFFFF);
	hdr->un.echo.sequence = htons(seq);
	fill_data(icmp_pkt + ICMP_HD_SIZE, size - ICMP_HD_SIZE);
	hdr->checksum = htons(compute_checksum(icmp_pkt, size));
}

void	send_icmp_req(struct ping *ping)
{
	if (sendto(
			ping->sockfd,
			ping->icmp_pkt,
			ping->data_size + ICMP_HD_SIZE,
			0,
			&ping->dst,
			sizeof(struct sockaddr)
		) == -1) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	++ping->npkt_sent;
}

void icmp_request(struct ping *ping)
{
	build_icmp_packet(	ping->icmp_pkt,
						ping->data_size + ICMP_HD_SIZE,
						ping->seq++);
	send_icmp_req(ping);
}
