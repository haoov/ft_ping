/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stats.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rasbbah <rsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 09:24:01 by rasbbah           #+#    #+#             */
/*   Updated: 2025/03/12 18:08:03 by rasbbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ft_ping.h"

void print_stats(struct ft_ping *ft_ping) {
	printf("--- %s ping statistics ---\n", ft_ping->hostname);
	printf("%d packets transmitted\n", ft_ping->npkt_sent);
}

void print_ping(const char *hostname, struct sockaddr dst, int data_size) {
	struct sockaddr_in	*addr_n;
	char				addr_p[16];

	addr_n = (struct sockaddr_in*)&dst;
	if (inet_ntop(
			AF_INET,
			&addr_n->sin_addr,
			addr_p,
			sizeof(struct sockaddr_in)
		) == NULL) {
		errx(EXIT_FAILURE, "%s", strerror(errno));
	}
	printf("PING %s (%s): %d data bytes\n", hostname, addr_p, data_size);
}
